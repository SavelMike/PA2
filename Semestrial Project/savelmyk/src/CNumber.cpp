// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"
#include <string>
#include <iterator>

CBigUnsigned& CBigUnsigned::operator+=(long long exp)
{
	int carry = 0;
	string s = to_string(exp);

	deque<unsigned char> res;
	deque<unsigned char>::reverse_iterator ad1 = this->m_data.rbegin();
	string::reverse_iterator ad2 = s.rbegin();
	for (; ad1 != this->m_data.rend() || ad2 != s.rend();) {
		unsigned char sum;
		char s1;
		char s2;
		
		if (ad1 == this->m_data.rend()) {
			s1 = 0;
		}
		else {
			s1 = *ad1;
			ad1++;
		}
		if (ad2 == s.rend()) {
			s2 = '0';
		}
		else {
			s2 = *ad2;
			ad2++;
		}
		sum = s1 + s2 - '0' + carry;
		carry = sum / 10;
		sum = sum % 10;
		res.push_front(sum);
	}
	if (carry != 0) {
		res.push_front(carry);
	}
	this->m_data = res;

	return *this;
}

ostream& operator <<(ostream& os, const CBigUnsigned& num)
{
	for (auto x : num.m_data) {
		unsigned char uc = x + '0';
		os << uc;
	}
	return os;
}

CNumber CNumber::operator +(const CNumber& a2) const 
{
	return CNumber();
}

CNumber CNumber::operator -(const CNumber& a2) const 
{ 
	return CNumber(); 
}

CNumber CNumber::operator *(const CNumber& a2) const 
{ 
	return CNumber(); 
}

CNumber CNumber::operator /(const CNumber& a2) const 
{ 
	return CNumber();
}

CNumber CNumber::operator %(const CNumber& a2) const 
{ 
	return CNumber(); 
}

void CNumber::append_mantissa(int digit)
{
	this->m_Mantissa.append_dig(digit);
}

void CNumber::append_exponent(int digit)
{
	this->m_Exp.append_dig(digit);
}

void CNumber::increment_exp(long long exp)
{
	this->m_Exp += exp;
}

ostream& operator <<(ostream& os, const CNumber& num)
{
	os << "Mantissa: " << num.m_Mantissa << endl; 
	os << "Exponent: " << num.m_Exp << endl;
	return os;
}
