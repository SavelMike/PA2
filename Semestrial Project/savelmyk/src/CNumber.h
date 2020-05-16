// CNumber used to handle numbers of arbitrary precision

#ifndef CNUMBER_H

#define CNUMBER_H
#include <vector>
#include <iostream>
#include <deque>

using namespace std;

class CBigUnsigned
{
public:
	CBigUnsigned() { ; }

	// +=
	CBigUnsigned& operator+=(long long exp);
	void append_dig(int digit) { m_data.push_back(digit); }
	int length() { return m_data.size(); }
	friend ostream& operator <<(ostream& os, const CBigUnsigned& num);
private:
	deque<unsigned char> m_data;
};

class CNumber {
private:
	bool m_Invalid;
	bool m_positive;
	// Sign of exponent
	bool m_exppositive;
	CBigUnsigned m_Mantissa;
	CBigUnsigned m_Exp;
public:
	CNumber() :m_Invalid(true), m_positive(true), m_exppositive(true) {}
	bool isinvalid() { return this->m_Invalid; }
	void set_valid() { this->m_Invalid = false; }
	CNumber operator +(const CNumber&) const;
	CNumber operator -(const CNumber&) const;
	CNumber operator *(const CNumber&) const;
	CNumber operator /(const CNumber&) const;
	CNumber operator %(const CNumber&) const;
	void append_mantissa(int digit);
	void append_exponent(int digit);
	int exponent_length() { return this->m_Exp.length(); }
	void increment_exp(long long exp);
	friend ostream& operator <<(ostream& os, const CNumber& num);
};
#endif
