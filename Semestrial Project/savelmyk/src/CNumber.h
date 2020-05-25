// CNumber used to handle numbers of arbitrary precision

#ifndef CNUMBER_H

#define CNUMBER_H
#include <vector>
#include <iostream>
#include <deque>

using namespace std;

class CBigInt
{
public:
	CBigInt(): m_data(), m_positive(true) { ; }
	CBigInt(int val);
	// +=
	CBigInt& operator+=(const CBigInt& diff);
	// -=
	CBigInt& operator-=(const CBigInt& diff);
	// +
	CBigInt operator+(const CBigInt& add2) const;
	// -
	CBigInt operator-(const CBigInt& sub2) const;
	// *
	CBigInt operator*(const CBigInt& multiplier) const;
	// <
	bool operator<(const CBigInt& a2) const;
	void tail_append(unsigned char digit) { m_data.push_back(digit); }
	void head_insert(unsigned char digit) { m_data.push_front(digit); }
	int length() const { return m_data.size(); }
	void set_negative() { m_positive = false; }
	void set_positive() { m_positive = true; }
	void set_sign(bool sign) { m_positive = sign; }
	bool get_sign() const { return m_positive; }
	void remove_leading_zeroes();
	void remove_tailing_zeroes();
	const deque<unsigned char>& get_data() const { return m_data; }
	int toInt() const;
	friend ostream& operator <<(ostream& os, const CBigInt& num);
private:
	deque<unsigned char> m_data;
	bool m_positive;
};

class CNumber {
private:
	bool m_Invalid;
	bool m_positive;
	CBigInt m_Mantissa;
	CBigInt m_Exp;
public:
	CNumber() :m_Invalid(true), m_positive(true), m_Mantissa(), m_Exp(0) {}
	bool isinvalid() { return m_Invalid; }
	void set_valid() { m_Invalid = false; }
	CNumber operator +(const CNumber&) const;
	CNumber operator -(const CNumber&) const;
	CNumber operator *(const CNumber&) const;
	CNumber operator /(const CNumber&) const;
	CNumber operator %(const CNumber&) const;
	void append_mantissa(int digit);
	void append_exponent(int digit);
	int exponent_length() { return m_Exp.length(); }
	void increment_exp(const CBigInt& exp);
	void set_expsign(bool sign) { m_Exp.set_sign(sign); }
	void remove_zeroes();
	void set_exponent(const CBigInt& exp) { m_Exp = exp; }
	friend ostream& operator <<(ostream& os, const CNumber& num);
};
#endif
