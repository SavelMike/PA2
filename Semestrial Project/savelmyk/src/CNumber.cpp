// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"
#include <string>
#include <iterator>

static CBigInt add(const CBigInt& v1, const CBigInt& v2)
{
	int carry = 0;

	CBigInt res;
	deque<unsigned char>::reverse_iterator ad1 = v1.get_data().rbegin();
	deque<unsigned char>::reverse_iterator ad2 = v2.get_data().rbegin();

	for (; ad1 != v1.get_data().rend() || ad2 != v2.get_data().rend(); ) {
		unsigned char sum;
		unsigned char s1;
		unsigned char s2;

		if (ad1 == v1.get_data().rend()) {
			s1 = 0;
		}
		else {
			s1 = *ad1;
			ad1++;
		}
		if (ad2 == v2.get_data.rend()) {
			s2 = 0;
		}
		else {
			s2 = *ad2;
			ad2++;
		}
		sum = s1 + s2 + carry;
		carry = sum / 10;
		sum = sum % 10;
		res.head_insert(sum);
	}
	if (carry != 0) {
		res.head_insert(carry);
	}

	return res;
}

static CBigInt sub(const CBigInt& v1, const CBigInt& v2)
{
	int carry = 0;

	CBigInt res;
	deque<unsigned char>::reverse_iterator ad1 = v1.get_data().rbegin();
	deque<unsigned char>::reverse_iterator ad2 = v2.get_data().rbegin();

	for (; ad1 != v1.get_data().rend() || ad2 != v2.get_data().rend(); ) {
		unsigned char sum;
		unsigned char s1;
		unsigned char s2;

		if (ad1 == v1.get_data().rend()) {
			s1 = 0;
		}
		else {
			s1 = *ad1;
			ad1++;
		}
		if (ad2 == v2.get_data.rend()) {
			s2 = 0;
		}
		else {
			s2 = *ad2;
			ad2++;
		}
		sum = s1 + s2 + carry;
		carry = sum / 10;
		sum = sum % 10;
		res.head_insert(sum);
	}
	if (carry != 0) {
		res.head_insert(carry);
	}

	return res;
}

CBigInt& CBigInt::operator+=(const CBigInt& diff)
{
	if (this->m_positive == diff.m_positive) {
		*this = add(*this, diff);
		diff.m_positive ? this->set_positive() : this->set_negative();
	}
	else {
		*this = sub(*this, diff);
	}
}

bool CBigInt::operator<(const CBigInt& a) const
{

	if (this->length() > a.length()) {
		return false;
	}
	if (this->length() < a.length()) {
		return true;
	}

	deque<unsigned char>::reverse_iterator ad1 = this->get_data().begin();
	deque<unsigned char>::reverse_iterator ad2 = a.get_data().begin();
	for (; ad1 != this->get_data().end(); ad1++, ad2++) {
		if (*ad1 < *ad2) {
			return true;
		}
	}
	return false;
}

CBigInt& CBigInt::operator-=(lo exp)
{
	int carry = 0;
	// Exp is less than zero
	string s = to_string(-exp);

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

ostream& operator <<(ostream& os, const CBigInt& num)
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

void CNumber::increment_exp(const CBigInt& exp)
{
	this->m_Exp += exp;
}

ostream& operator <<(ostream& os, const CNumber& num)
{
	os << "Mantissa: " << num.m_Mantissa << endl; 
	os << "Exponent: " << (num.m_exppositive ? '+' : '-') << num.m_Exp << endl;
	return os;
}
