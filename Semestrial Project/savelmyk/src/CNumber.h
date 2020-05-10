// CNumber used to handle numbers of arbitrary precision
#include <vector>
#include <iostream>

class CNumber {
private:
	bool m_Invalid;
	bool m_positive;
	vector<int> m_IntPart;
	vector<int> m_FractPart;
	bool m_exppositive;
	vector<int> m_Exp;
public:
	CNumber() :m_Invalid(true) {}
	CNumber(int el) :m_Invalid(false) { m_IntPart.push_back(el); }
	bool isinvalid() { return m_Invalid; }
	CNumber operator +(const CNumber&) const { return CNumber(1); }
	CNumber operator -(const CNumber&) const { return CNumber(1); }
	CNumber operator *(const CNumber&) const { return CNumber(1); }
	CNumber operator /(const CNumber&) const { return CNumber(1); }
	CNumber operator %(const CNumber&) const { return CNumber(1); }
	friend ostream& operator <<(ostream& os, const CNumber& num);
};