// CNumber used to handle numbers of arbitrary precision

#ifndef CNUMBER_H

#define CNUMBER_H
#include <vector>
#include <iostream>

using namespace std;

class CNumber {
private:
	bool m_Invalid;
	bool m_positive;
	vector<int> m_IntPart;
	vector<int> m_FractPart;
	bool m_exppositive;
	vector<int> m_Exp;
public:
	CNumber() :m_Invalid(true), m_positive(true), m_exppositive(true) {}
	CNumber(int el) :m_Invalid(false), m_positive(true), m_exppositive(true) { m_IntPart.push_back(el); }
	bool isinvalid() { return m_Invalid; }
	CNumber operator +(const CNumber&) const;
	CNumber operator -(const CNumber&) const;
	CNumber operator *(const CNumber&) const;
	CNumber operator /(const CNumber&) const;
	CNumber operator %(const CNumber&) const;
	friend ostream& operator <<(ostream& os, const CNumber& num);
};

#endif
