// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"
#include <string>
#include <iterator>
#include <cassert>

static CBigInt add(const CBigInt& v1, const CBigInt& v2)
{
	int carry = 0;

	CBigInt res;

	res.set_sign(v1.get_sign());
	deque<unsigned char>::const_reverse_iterator ad1 = v1.get_data().rbegin();
	deque<unsigned char>::const_reverse_iterator ad2 = v2.get_data().rbegin();

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
		if (ad2 == v2.get_data().rend()) {
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
	deque<unsigned char>::const_reverse_iterator minuend;
	deque<unsigned char>::const_reverse_iterator subtrahend;
	deque<unsigned char>::const_reverse_iterator endup;
	deque<unsigned char>::const_reverse_iterator enddown;
	bool sign;

	if (v1 < v2) {
		if (v1.get_sign() == v2.get_sign()) {
			sign = !v2.get_sign();
		}
		else {
			sign = v2.get_sign();
		}
		minuend = v2.get_data().rbegin();
		subtrahend = v1.get_data().rbegin();
		endup = v2.get_data().rend();
		enddown = v1.get_data().rend();
	}
	else {
		sign = v1.get_sign();
		minuend = v1.get_data().rbegin(); 
		subtrahend = v2.get_data().rbegin();
		endup = v1.get_data().rend();
		enddown = v2.get_data().rend();
	}

	CBigInt res;

	res.set_sign(sign);
	int carry1 = 0;
	int carry2 = 0;
	
	for (; minuend != endup; ) {
		unsigned char dif;
		unsigned char s1;
		unsigned char s2;

		s1 = *minuend;
		minuend++;
		
		if (subtrahend == enddown) {
			s2 = 0;
		}
		else {
			s2 = *subtrahend;
			subtrahend++;
		}
		carry2 = 0;
		if (s1 < (s2 + carry1)) {
			s1 += 10;
			carry2 = 1;
		}
		dif = s1 - s2 - carry1;
		carry1 = carry2;
		res.head_insert(dif);
	}

	return res;
}

CBigInt& CBigInt::operator+=(const CBigInt& diff)
{
	if (this->m_positive == diff.m_positive) {
		*this = add(*this, diff);
	}
	else {
		*this = sub(*this, diff);
	}
	return *this;
}

CBigInt& CBigInt::operator-=(const CBigInt& diff)
{
	if (this->m_positive == diff.m_positive) {
		*this = sub(*this, diff);
	}
	else {
		*this = add(*this, diff);
	}

	return *this;
}

bool CBigInt::operator<(const CBigInt& a) const
{
	CBigInt res;
	deque<unsigned char>::const_iterator i1 = this->m_data.begin();
	deque<unsigned char>::const_iterator i2 = a.m_data.begin();
	
	while (i1 != this->m_data.end() && *i1 == 0) {
		i1++;
	}
	int l1 = this->m_data.end() - i1;
	
	while (i2 != a.m_data.end() && *i2 == 0) {
		i2++;
	}
	int l2 = a.m_data.end() - i2;
	
	if (l1 > l2) {
		return false;
	}
	if (l1 < l2) {
		return true;
	}
	for (; i1 != this->m_data.end(); i1++, i2++) {
		if (*i1 < *i2) {
			return true;
		}
		if (*i1 > *i2) {
			return false;
		}
	}
	return false;
}

void CBigInt::remove_leading_zeroes()
{
	if (this->m_data.size() == 0) {
		cout << "Empty deque" << endl;
		exit(1);
	}
	
	while (1) {
		if (this->m_data[0] == 0 && this->m_data.size() > 1) {
			this->m_data.pop_front();
			continue;
		}
		break;
	}
}

void CBigInt::remove_tailing_zeroes()
{
	if (this->m_data.size() == 0) {
		cout << "Empty deque" << endl;
		exit(1);
	}
	
	while (1) {
		if (this->m_data[this->m_data.size() - 1] == 0 && this->m_data.size() > 1) {
			this->m_data.pop_back();
			continue;
		}
		break;
	}
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
	this->m_Mantissa.tail_append(digit);
}

void CNumber::append_exponent(int digit)
{
	this->m_Exp.tail_append(digit);
}

void CNumber::increment_exp(const CBigInt& exp)
{
	this->m_Exp += exp;
}

void CNumber::remove_zeroes()
{
	this->m_Mantissa.remove_leading_zeroes();
	this->m_Mantissa.remove_tailing_zeroes();
	this->m_Exp.remove_leading_zeroes();
}

ostream& operator <<(ostream& os, const CNumber& num)
{
	os << "Mantissa: " << num.m_Mantissa << endl; 
	os << "Exponent: " << (num.m_Exp.get_sign() ? '+' : '-') << num.m_Exp << endl;
	return os;
}
