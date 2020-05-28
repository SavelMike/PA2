// CNumber used to handle numbers of arbitrary precision

#include "CNumber.h"
#include <string>
#include <iterator>
#include <cassert>

CBigInt::CBigInt(int val)
{
	this->m_positive = (val >= 0);
	if (val < 0) {
		val *= (-1);
	}
	while (1) {
		int rem = val % 10;
		val /= 10;
		this->m_data.push_front(rem);
		if (val == 0) {
			break;
		}
	}
}

// Returns abs(v1) + abs(v2)
static CBigInt add(const CBigInt& v1, const CBigInt& v2)
{
	int carry = 0;
	CBigInt res;
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

// This function sums v1 and v2. These are aligned by highest digit, etc: 
// 1234567
// 89
// Assuming 89 is in fact 8900000
static CBigInt add2(const CBigInt& v1, const CBigInt& v2)
{
	deque<unsigned char>::const_reverse_iterator i1;
	deque<unsigned char>::const_reverse_iterator i1end;
	deque<unsigned char>::const_reverse_iterator i2;
	int nzeroes;
	CBigInt res;
	int carry = 0;

	if (v1.get_data().size() < v2.get_data().size()) {
		i1 = v2.get_data().rbegin();
		i1end = v2.get_data().rend();
		nzeroes = v2.get_data().size() - v1.get_data().size();
		i2 = v1.get_data().rbegin();
	}
	else {
		i1 = v1.get_data().rbegin();
		i1end = v1.get_data().rend();
		nzeroes = v1.get_data().size() - v2.get_data().size();
		i2 = v2.get_data().rbegin();
	}

	for (; i1 != i1end; i1++) {
		unsigned char a1 = *i1;
		unsigned char a2;
		if (nzeroes > 0) {
			a2 = 0;
			nzeroes--;
		}
		else {
			a2 = *i2;
			i2++;
		}
		unsigned char sum = a1 + a2 + carry;
		res.head_insert(sum % 10);
		carry = sum / 10;
	}
	if (carry != 0) {
		res.head_insert(carry);
	}
	
	return res;
}

// Returns abs(v1) - abs(v2)
static CBigInt sub(const CBigInt& v1, const CBigInt& v2)
{
	deque<unsigned char>::const_reverse_iterator minuend;
	deque<unsigned char>::const_reverse_iterator subtrahend;
	deque<unsigned char>::const_reverse_iterator endup;
	deque<unsigned char>::const_reverse_iterator enddown;

	minuend = v1.get_data().rbegin();
	endup = v1.get_data().rend();
	subtrahend = v2.get_data().rbegin();
	enddown = v2.get_data().rend();

	CBigInt res;

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

// This function subtracts v2 from v1. abs(v1) >= abs(v2). These are aligned by highest digit, etc: 
// 89
// 1234567
// Assuming 89 is in fact 8900000
static CBigInt sub2(const CBigInt& v1, const CBigInt& v2)
{
	deque<unsigned char>::const_reverse_iterator minuend;
	deque<unsigned char>::const_reverse_iterator subtrahend;
	deque<unsigned char>::const_reverse_iterator endup;
	deque<unsigned char>::const_reverse_iterator enddown;

	minuend = v1.get_data().rbegin();
	endup = v1.get_data().rend();
	subtrahend = v2.get_data().rbegin();
	enddown = v2.get_data().rend();
	int nzeroes = v1.get_data().size() - v2.get_data().size();

	CBigInt res;

	int carry1 = 0;
	int carry2 = 0;

	for (; minuend != endup; ) {
		unsigned char dif;
		unsigned char s1;
		unsigned char s2;
		
		if (nzeroes < 0) {
			s1 = 0;
			nzeroes++;
		}
		else {
			s1 = *minuend;
			minuend++;
		}
		if (nzeroes > 0) {
			s2 = 0;
			nzeroes--;
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

CBigInt CBigInt::operator+(const CBigInt& add2) const
{
	if (this->is_positive() && add2.is_positive()) {
		return add(*this, add2);
	}
	if (!this->is_positive() && !add2.is_positive()) {
		CBigInt res = add(*this, add2);
		res.set_negative();
		return res;
	}
	int rc = this->cmp_abs(add2);
	if (rc < 0) {
		// abs(this) < abs(add2)
		CBigInt res = sub(add2, *this);
		res.set_sign(add2.get_sign());
		return res;
	}
	else {
		// abs(this) >= abs(add2)
		CBigInt res = sub(*this, add2);
		res.set_sign(this->get_sign());
		return res;
	}

}

CBigInt& CBigInt::operator+=(const CBigInt& diff)
{
	CBigInt res = *this + diff;
	*this = res;
	
	return *this;
}

CBigInt CBigInt::operator-(const CBigInt& sub2) const
{ 
	if (this->is_positive() && sub2.is_positive()) {
		if (this->cmp_abs(sub2) < 0) {
			// abs(this) < abs(sub2)
			CBigInt res = sub(sub2, *this);
			res.set_negative();
			return res;
		} else {
			CBigInt res = sub(*this, sub2);
			res.set_positive();
			return res;
		}
	}
	if (!this->is_positive() && !sub2.is_positive()) {
		int rc = this->cmp_abs(sub2);
		if (rc <= 0) {
			// abs(this) <= abs(sub2)
			CBigInt res = sub(sub2, *this);
			res.set_positive();
			return res;
		}
		else {
			CBigInt res = sub(*this, sub2);
			res.set_negative();
			return res;
		}
	}
	CBigInt res = add(*this, sub2);
	res.set_sign(this->get_sign());

	return res;
}

CBigInt& CBigInt::operator-=(const CBigInt& diff)
{
	CBigInt res = *this - diff;
	*this = res;

	return *this;
}

CBigInt CBigInt::operator*(const CBigInt& multiplier) const
{
	unsigned char z;
	unsigned char rem;
	unsigned char quotient = 0;
	CBigInt res;
	int c = 0;
	deque<unsigned char>::const_reverse_iterator i1 = this->m_data.rbegin();
	
	for (; i1 != this->m_data.rend(); i1++) {
		CBigInt tmpres;
		for (int i = 0; i < c; i++) {
			tmpres.head_insert(0);
		}
		c++;
		quotient = 0;
		deque<unsigned char>::const_reverse_iterator i2 = multiplier.m_data.rbegin();
		for (; i2 != multiplier.m_data.rend(); i2++) {
			z = *i1 * *i2 + quotient;
			quotient = z / 10;
			rem = z % 10;
			tmpres.head_insert(rem);
		}
		if (quotient != 0) {
			tmpres.head_insert(quotient);
		}
		res += tmpres;
	}
	
	res.set_sign(this->get_sign() == multiplier.get_sign());
	return res;
}

// Return value:
// -1: abs(this) < abs(a2)
//  0: abs(this) == abs(a2)
//  1: abs(this) > abs(a2)
int CBigInt::cmp_abs(const CBigInt& a2) const
{
	deque<unsigned char>::const_iterator i1 = this->m_data.begin();
	deque<unsigned char>::const_iterator i2 = a2.m_data.begin();

	// Skip leading zeroes
	while (i1 != this->m_data.end() && *i1 == 0) {
		i1++;
	}
	int l1 = this->m_data.end() - i1;

	// Skip leading zeroes in a2
	while (i2 != a2.m_data.end() && *i2 == 0) {
		i2++;
	}
	int l2 = a2.m_data.end() - i2;

	if (l1 > l2) {
		return 1;
	}
	if (l1 < l2) {
		return -1;
	}
	
	for (; i1 != this->m_data.end(); i1++, i2++) {
		if (*i1 < *i2) {
			return -1;
		}
		if (*i1 > *i2) {
			return 1;
		}
	}
	
	return 0;
}

// Return values:
// true if this < a
// false otherwise
bool CBigInt::operator<(const CBigInt& a) const
{
	if (this->get_sign() == true && a.get_sign() == false) {
		return false;
	}
	if (this->get_sign() == false && a.get_sign() == true) {
		return true;
	}

	// Both operands have the same sign, compare absolute values
	int res = this->cmp_abs(a);
	if (res == -1) {
		return this->get_sign();
	}
	if (res == 1) {
		return !this->get_sign();
	}
	return false;
}

// Returns number of deleted leading zeroes
int CBigInt::remove_leading_zeroes()
{
	if (this->m_data.size() == 0) {
		throw "Not a number";
	}
	
	int zeroes = 0;
	while (1) {
		if (this->m_data[0] == 0 && this->m_data.size() > 1) {
			this->m_data.pop_front();
			zeroes++;
			continue;
		}
		break;
	}

	return zeroes;
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

int CBigInt::toInt() const
{
	if (this->m_data.size() > 9) {
		cout << "Too big number." << endl;
		exit(1);
	}
	deque<unsigned char>::const_reverse_iterator it;
	int res = 0;
	int power10 = 1;

	it = this->m_data.rbegin();
	for (; it != this->m_data.rend(); it++) {
		res += power10 * *it;
		power10 *= 10;
	}

	return res;
}

ostream& operator <<(ostream& os, const CBigInt& num)
{
	if (num.m_positive == false) {
		os << '-';
	}
	for (auto x : num.m_data) {
		unsigned char uc = x + '0';
		os << uc;
	}
	return os;
}

CNumber CNumber::add_abs(const CNumber& a2) const 
{
	CNumber res;
	CNumber addendum1; // Addendum with smaller exponent
	const CNumber *addendum2;
	CBigInt nzeroes;

	// Fix me
	if (this->m_Exp < a2.m_Exp) {
		addendum1 = *this;
		addendum2 = &a2;
		nzeroes = a2.m_Exp - this->m_Exp;
	}
	else {
		addendum1 = a2;
		addendum2 = this;
		nzeroes = this->m_Exp - a2.m_Exp;
	}
	for (CBigInt i(0); i < nzeroes; i += CBigInt(1)) {
		addendum1.m_Mantissa.head_insert(0);
	}
	res.m_Mantissa = add2(addendum1.m_Mantissa, addendum2->m_Mantissa);
	res.m_Exp = addendum2->m_Exp;

	return res;
}

CNumber CNumber::operator+(const CNumber& a2) const
{
	CNumber res;
	
	if (this->m_positive == a2.m_positive) {
		res = this->add_abs(a2);
		res.m_positive = this->m_positive;
		return res;
	}
	res = this->sub_abs(a2);
	int rc = this->cmp_abs(a2);
	if (rc < 0) {
		// abs(this) < abs(a2)
		res.m_positive = a2.m_positive;
	}
	else {
		// abs(this) >= abs(a2)
		res.m_positive = this->m_positive;
	}

	return res;
}

CNumber CNumber::sub_abs(const CNumber& a2) const 
{
	CBigInt nzeroes;
	CNumber res;
	CNumber subtrahend;
	const CNumber *minuend;
	int rc = this->cmp_abs(a2);
	
	if (rc == -1) {
		// abs(a2) > abs(this)
		nzeroes = a2.m_Exp - this->m_Exp;
		minuend = &a2;
		subtrahend = *this;
	}
	else {
		nzeroes = this->m_Exp - a2.m_Exp;
		minuend = this;
		subtrahend = a2;
	}
	
	for (CBigInt i(0); i < nzeroes; i += CBigInt(1)) {
		subtrahend.m_Mantissa.head_insert(0);
	}
	res.m_Mantissa = sub2(minuend->m_Mantissa, subtrahend.m_Mantissa);
	res.m_Exp = minuend->m_Exp;
	
	int lzero = res.m_Mantissa.remove_leading_zeroes();
	res.m_Exp -= CBigInt (lzero);
	
	if (this->m_positive == a2.m_positive) {
		if (rc < 0) {
			res.m_positive = !a2.m_positive;
		}
		else {
			// abs(this) >= abs(a2)
			res.m_positive = this->m_positive;
		}
	}
	return res;
}

CNumber CNumber::operator-(const CNumber& a2) const
{
	CNumber res;
	
	if (this->m_positive == a2.m_positive) {
		res = this->sub_abs(a2);
		int rc = this->cmp_abs(a2);
		if (rc < 0) {
			// abs(this) < abs(a2)
			res.m_positive = !a2.m_positive;
		}
		else {
			res.m_positive = this->m_positive;
		}
		return res;
	}
	res = this->add_abs(a2);
	res.m_positive = this->m_positive;
	
	return res;
}

CNumber CNumber::operator *(const CNumber& a2) const 
{ 
	CNumber res;

	res.m_Exp = this->m_Exp;
	res.m_Exp += a2.m_Exp;
	
	res.m_Mantissa = this->m_Mantissa * a2.m_Mantissa;
	res.m_Exp -= CBigInt(this->m_Mantissa.length() + a2.m_Mantissa.length() - res.m_Mantissa.length());
	
	return res; 
}

CNumber CNumber::operator /(const CNumber& a2) const 
{ 
	return CNumber();
}

CNumber CNumber::operator %(const CNumber& a2) const 
{ 
	return CNumber(); 
}

// Returns:
//  1 if abs(this) > abs(a2)
//  0 if abs(this) = abs(a2)
// -1 if abs(this) < abs(a2)
int CNumber::cmp_abs(const CNumber& a2) const
{
	if (this->m_Exp < a2.m_Exp) {
		return -1;
	}
	if (a2.m_Exp < this->m_Exp){
		return 1;
	}
	
	// Compare mantisses
	deque<unsigned char>::const_iterator it1 = this->m_Mantissa.get_data().begin();
	deque<unsigned char>::const_iterator it2 = a2.m_Mantissa.get_data().begin();
	for (; it1 != this->m_Mantissa.get_data().end() || it2 != a2.m_Mantissa.get_data().end();) {
		unsigned char uc1;
		unsigned char uc2;
		if (it1 == this->m_Mantissa.get_data().end()) {
			uc1 = 0;
		}
		else {
			uc1 = *it1;
			it1++;
		}
		if (it2 == a2.m_Mantissa.get_data().end()) {
			uc2 = 0;
		}
		else {
			uc2 = *it2;
			it2++;
		}
		if (uc1 == uc2) {
			continue;
		}
		if (uc1 < uc2) {
			return -1;
		}
		else {
			return 1;
		}
	}
	
	return 0;
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
	CNumber num1 = num;
	num1.remove_zeroes();
	if (!num1.m_positive) {
		os << "-";
	}
	if (!(CBigInt(0) < num1.m_Exp)) {
		// Negative exponent
		os << "0.";
		if (num1.m_Exp < CBigInt(255)) {
			int n = num1.m_Exp.toInt();
			os << string(n, '0') << num1.m_Mantissa;
		}
		else {
			os << num1.m_Mantissa << 'E' << (num1.m_Exp.get_sign() ? "+" : "-") << num1.m_Exp;
		}
	}
	else {
		// Positive exponent
		int n = num1.m_Exp.toInt(); // TODO: make n CBigInt
		int pos = 0; // TODO: make pos CBigInt
		for (auto x : num1.m_Mantissa.get_data()) {
			x += '0';
			os << x;
			pos++;
			if (pos == n && num1.m_Mantissa.length() > n) {
				os << ".";
			}
		}
		if (n > pos) {
			os << string(n - pos, '0');
		}
	}

	return os;
}
