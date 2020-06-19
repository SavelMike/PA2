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
	m_ntoprint = 0;
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

// This function sums v1 and v2. These are aligned by least decimal place, etc: 
// 12345000
//		777
static CBigInt column_add(const CBigInt& v1, const CBigInt& v2, bool& extrarank)
{
	deque<unsigned char>::const_reverse_iterator i1;
	deque<unsigned char>::const_reverse_iterator i1end;
	deque<unsigned char>::const_reverse_iterator i2;
	deque<unsigned char>::const_reverse_iterator i2end;
	int nzeroes;
	CBigInt res;
	int carry = 0;

	i1 = v1.get_data().rbegin();
	i1end = v1.get_data().rend();
	i2 = v2.get_data().rbegin();
	i2end = v2.get_data().rend();
	for (; i1 != i1end || i2 != i2end;) {
		unsigned char a1;
		unsigned char a2;
		if (i1 != i1end) {
			a1 = *i1;
			i1++;
		}
		else {
			a1 = 0;
		}
		if (i2 != i2end) {
			a2 = *i2;
			i2++;
		}
		else {
			a2 = 0;
		}

		unsigned char sum = a1 + a2 + carry;
		res.head_insert(sum % 10);
		carry = sum / 10;
	}
	if (carry != 0) {
		extrarank = true;
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

// This function subtracts v2 from v1. abs(v1) >= abs(v2).
// returns v1-v2
static CBigInt column_sub(const CBigInt& v1, const CBigInt& v2)
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
		s1 = *minuend;
		minuend++;
		if (subtrahend != enddown) {
			s2 = *subtrahend;
			subtrahend++;
		}
		else {
			s2 = 0;
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

// Multiplication by column
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

// Compare absolute values of *this and a2 
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

// Remove leading zeroes from deque<unsigned char> m_data
// Last single zero remains
// ie, 00001230->1230, 00000->0
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

// Remove tailing zeroes from deque<unsigned char> m_data
// Last single zero remains
// ie, 12300->123, 00000->0
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

// Convert CBigInt objects to integer, should not be used for more than 9 digits long number
// due to 32 bit integer limitation
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

// Operator output (friend of class CBigInt):
// if num.m_ntoprint is not 0 do not print more digits than that number
// Print absolute value
ostream& operator <<(ostream& os, const CBigInt& num)
{
	int count = 0;

	for (auto x : num.m_data) {
		if (count == num.m_ntoprint && num.m_ntoprint != 0) {
			break;
		}
		count++;
		unsigned char uc = x + '0';
		os << uc;
	}
	return os;
}

// CNumber::add_abs()
// Auxilliary method to find sum of absolute values
// Detects which is bigger by absolute value, and returns abs(*this) - abs(a2) (if abs(*this) > abs(a2)
// or abs(a2) - abs(*this)
CNumber CNumber::add_abs(const CNumber& a2) const 
{
	CNumber res;
	CNumber addendum1; // Addendum with smaller exponent
	CNumber addendum2;
	CBigInt nzeroes;

	addendum1 = *this;
	addendum2 = a2;
	
	// res.m_Exp is a bigger of addendum exponents   
	if (this->m_Exp < a2.m_Exp) {
		nzeroes = a2.m_Exp - this->m_Exp;
		res.m_Exp = a2.m_Exp;
	}
	else {
		nzeroes = this->m_Exp - a2.m_Exp;
		res.m_Exp = this->m_Exp;
	}
	
	if (CBigInt(static_cast<int>(CConstants::EXP_DIFFERENCE_LIMIT)) < nzeroes) {
		// Calculation of the expression like "1e+1000000000 + 1e-1000000000" leads to mantissa looking like
		// "1000*000.000*0001" which is very long. So EXP_DIFFERENCE_LIMIT is introduced to 
		// limit exponent difference for additions and subtractions to some reasonable value - 4096 
		throw "Difference of exponents is too big";
	}
	
	// To do "addition in columns" align operands by the least decimal place, 
	// ie. if addendum1 = [m = 1234, exp = 4] and addendum2 = [m = 1, exp = -2] 
	// we have to append mantissa of addendum1 to 123400    
	CBigInt a = CBigInt(addendum1.m_Mantissa.length()) - addendum1.m_Exp;
	CBigInt b = CBigInt(addendum2.m_Mantissa.length()) - addendum2.m_Exp;
	a = a - b;
	if (a < 0) {
		a = a * CBigInt(-1);
		for (CBigInt i(0); i < a; i += CBigInt(1)) {
			addendum1.m_Mantissa.tail_append(0);
		}
	}
	else {
		for (CBigInt i(0); i < a; i += CBigInt(1)) {
			addendum2.m_Mantissa.tail_append(0);
		}
	}
	
	bool extrarank = false;
	// Do the addition in columns 
	res.m_Mantissa = column_add(addendum1.m_Mantissa, addendum2.m_Mantissa, extrarank);
	if (extrarank) {
		// res.m_Exp is to be increased by one
		res.m_Exp += CBigInt(1);
	}

	return res;
}

// Return:
//   *this + a2
// Calculates absolute values of result and assign the sign
CNumber CNumber::operator+(const CNumber& a2) const
{
	CNumber res;
	
	// One of addendums is zero
	if (this->isZero()) {
		return a2;
	}
	if (a2.isZero()) {
		return *this;
	}
	
	// Sum of addendums with same signs 
	if (this->m_positive == a2.m_positive) {
		res = this->add_abs(a2);
		res.m_positive = this->m_positive;
		res.remove_zeroes();
		return res;
	}
	// Sum of addendums with different signs
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

	res.remove_zeroes();
	return res;
}

// CNumber::sub_abs()
// Auxilliary method to find difference of absolute values
// Detects which is bigger by absolute value, and returns abs(*this) - abs(a2) (if abs(*this) > abs(a2)
// or abs(a2) - abs(*this)
CNumber CNumber::sub_abs(const CNumber& a2) const 
{
	CBigInt nzeroes;
	CNumber res;
	CNumber subtrahend;
	CNumber minuend;
	
	// Find subtrahend and minuend
	int rc = this->cmp_abs(a2);
	if (rc == -1) {
		// abs(a2) > abs(this)
		nzeroes = a2.m_Exp - this->m_Exp;
		minuend = a2;
		subtrahend = *this;
	}
	else {
		nzeroes = this->m_Exp - a2.m_Exp;
		minuend = *this;
		subtrahend = a2;
	}
	
	if (CBigInt(static_cast<int>(CConstants::EXP_DIFFERENCE_LIMIT)) < nzeroes) {
		// Calculation of the expression like "1e+1000000000-1e-1000000000" leads to mantissa looking like
		// "1000*000.000*0001" which is very long. So EXP_DIFFERENCE_LIMIT is introduced to 
		// limit exponent difference for additions and subtractions to some reasonable value - 4096 
		throw "Difference of exponents is too big";
	}

	// Minuend - subtrahend
	
	// To do "subtraction in columns" align operands by the least decimal place, 
	// ie. if minuend = [m = 1234, exp = 4] and subtrahend = [m = 1, exp = -2] 
	// we have to append mantissa of menuend to 123400    
	CBigInt m = CBigInt(minuend.m_Mantissa.length()) - minuend.m_Exp;
	CBigInt s = CBigInt(subtrahend.m_Mantissa.length()) - subtrahend.m_Exp;
	m = m - s;
	if (m < 0) {
		m = m * CBigInt(-1);
 		for (CBigInt i(0); i < m; i += CBigInt(1)) {
			minuend.m_Mantissa.tail_append(0);
		}
	}
	else {
		for (CBigInt i(0); i < m; i += CBigInt(1)) {
			subtrahend.m_Mantissa.tail_append(0);
		}
	}
	
	// Do the subtraction in columns 
	res.m_Mantissa = column_sub(minuend.m_Mantissa, subtrahend.m_Mantissa);
	res.m_Exp = minuend.m_Exp;
	
	// Optimize result by removing lzeroes and mantissa of results
	int lzero = res.m_Mantissa.remove_leading_zeroes();
	res.m_Exp -= CBigInt (lzero);
	 
	return res;
}

// CNumber::operator- ()
// Return:
//   *this - a2
// Calculates absolute values of result and assign the sign
CNumber CNumber::operator-(const CNumber& a2) const
{
	CNumber res;

	// Special case: *this or a2 is 0
	if (this->isZero()) {
		// 0 - a2 = -a2
		res = a2;
		res.m_positive = !a2.m_positive;
		return res;
	}
	if (a2.isZero()) {
		return *this;
	}
	
	if (this->m_positive == a2.m_positive) {
		// Both operands are the same sign so abs(res) is calculated by subtraction
		res = this->sub_abs(a2);
		// Define sign of result
		int rc = this->cmp_abs(a2);
		if (rc < 0) {
			// abs(this) < abs(a2)
			res.m_positive = !a2.m_positive;
		}
		else {
			res.m_positive = this->m_positive;
		}
		res.remove_zeroes();
		return res;
	}
	
	// Operands with different signs so abs(res) is calculated by addition
	res = this->add_abs(a2);
	// Sign of result is sign of this
	res.m_positive = this->m_positive;
	res.remove_zeroes();
	
	return res;
}

// CNumber::operator* ()
// return:
//		*this * a2
CNumber CNumber::operator *(const CNumber& a2) const 
{ 
	CNumber res;

	// If any factor is zero, result is zero
	if (this->isZero()) {
		return *this;
	}
	if (a2.isZero()) {
		return a2;
	}

	// Res is positive if factors have the same sign
	res.m_positive = (this->m_positive == a2.m_positive);

	res.m_Exp = this->m_Exp;
	res.m_Exp += a2.m_Exp;
	
	res.m_Mantissa = this->m_Mantissa * a2.m_Mantissa;
	// Correct exponent by result
	// ie. [m = 25, exp = 2] * [m = 5, exp = 1] = [m = 125, exp = 3]
	//	   [m = 25, exp = 2] * [m = 3, exp = 1] = [m = 75, exp = 3] not correct. exp has to be 3 - 1
	res.m_Exp -= CBigInt(this->m_Mantissa.length() + a2.m_Mantissa.length() - res.m_Mantissa.length());

	res.remove_zeroes();
	return res; 
}

// CNumber::operator/ ()
// return:
//		*this / a2
CNumber CNumber::operator /(const CNumber& a2) const 
{
	// Sanity check
	if (a2.isZero()) {
		throw "Division by zero";
	}
	if (this->isZero()) {
		return *this;
	}

	CBigInt dividend;
	CBigInt divider = a2.m_Mantissa;
	CNumber res;

	// Res is positive if dividend and divider have the same sign 
	res.m_positive = (this->m_positive == a2.m_positive);

	res.set_exponent(this->m_Exp - a2.m_Exp);
	
	bool first_iteration = true;
	deque<unsigned char>::const_iterator it1 = this->m_Mantissa.get_data().begin();
	CBigInt ndigits(0); // Number of digits in mantissa
	
	while (1) {
		// Build dividend
		while (1) {
			unsigned char c;
			if (it1 != this->m_Mantissa.get_data().end()) {
				c = *it1;
				it1++;
			}
			else {
				c = 0;
			}
			dividend.tail_append(c);
			int rc = dividend.cmp_abs(divider);
			if (rc < 0) {
				res.m_Mantissa.tail_append(0);
				continue;
			}
			break;
		}
		if (first_iteration) {
			// On first iteration we have to correct exponent of result
			// For example: [m = 256, exp = 3] / [m = 16, exp = 2] = [m = 16, exp = 2]
			// res.m_Exp has to be increased by 1 as 25 > 16
			if (dividend.length() == divider.length()) {
				res.m_Exp += CBigInt(1);
			}
			first_iteration = false;
		}

		// Division
		int digit = 0;
		while (1) {
			dividend -= divider;
			digit++;
			int rc = dividend.cmp_abs(divider);
			if (rc < 0) {
				break;
			}
			continue;
		}
		res.m_Mantissa.tail_append(digit);
		ndigits += CBigInt(1); 
		int bc_scale = static_cast<int>(CConstants::BC_SCALE);
		if (ndigits.cmp_abs(res.m_Exp + CBigInt(bc_scale)) == 0) {
			// Break endless division so that we have at least BC_SCALE digits after dot
			break;
		}

		if ((dividend.cmp_abs(CBigInt(0)) == 0) && it1 == this->m_Mantissa.get_data().end()) {
			// Division without remainder 
			break;
		}
	}

	res.m_Zero = false;
	res.m_Mantissa.remove_leading_zeroes();
	return res;
}

// Operator % works with positive integers
bool CNumber::is_positive_integer() const
{
	CBigInt m_length(this->m_Mantissa.length());
	return ((m_length.cmp_abs(this->m_Exp) <= 0) && (this->m_positive) && this->m_Exp.is_positive() && !this->isZero());
}

// CNumber::operator% ()
// For positive integers only
// return:
//		*this mod a2, ie. 5 % 3 = 2
CNumber CNumber::operator %(const CNumber& a2) const
{ 
	if (!this->is_positive_integer() || !a2.is_positive_integer()) {
		throw "Invalid operand of mod operation";
	}

	// Convert divider(a2) from mantissa - exponent form to normal notation, 
	// ie. a2.m_Mantissa = 2, a2.m_exp = 4 then divider = 2000
	CBigInt divider = a2.m_Mantissa;
	for (CBigInt i(a2.m_Mantissa.length()); i < a2.m_Exp; i += CBigInt(1)) {
		divider.tail_append(0);
	}

	CBigInt dividend;
	CBigInt exp = this->m_Exp;

	deque<unsigned char>::const_iterator it1 = this->m_Mantissa.get_data().begin();
	while (1) {
		// Build dividend
		while (1) {
			if (exp.cmp_abs(CBigInt(0)) == 0) {
				dividend.remove_leading_zeroes();
				CNumber res;
				res.m_positive = true;
				res.m_Mantissa = dividend;
				res.m_Exp = CBigInt(dividend.length());
				res.m_Zero = (dividend.cmp_abs(CBigInt(0)) == 0);
				// res = *this % a2
				return res;
			}
			exp -= CBigInt(1);
			unsigned char c;
			if (it1 != this->m_Mantissa.get_data().end()) {
				c = *it1;
				it1++;
			}
			else {
				c = 0;
			}
			dividend.tail_append(c);
			int rc = dividend.cmp_abs(divider);
			if (rc < 0) {
				// Dividend < divider
				continue;
			}
			break;
		}

		// Division
		while (1) {
			dividend -= divider;
			int rc = dividend.cmp_abs(divider);
			if (rc < 0) {
				// if dividend < divider
				break;
			}
			continue;
		}
	}
	throw "Unexpected return from mod operator";
}

// Compares absolute values of *this and a2
// Returns:
//  1 if abs(this) > abs(a2)
//  0 if abs(this) = abs(a2)
// -1 if abs(this) < abs(a2)
int CNumber::cmp_abs(const CNumber& a2) const
{
	// Special case:
	//		one of operands is zero
	if (this->isZero()) {
		if (a2.isZero()) {
			return 0;
		}
		else {
			return -1;
		}
	}
	if (a2.isZero()) {
		return 1;
	}
	
	// Compare CBigInts with CBigInt::operator <
	if (this->m_Exp < a2.m_Exp) {
		// Number with least exponent is smaller 
		return -1;
	}
	if (a2.m_Exp < this->m_Exp) {
		// Number with least exponent is smaller
		return 1;
	}
	
	// Compare mantisses going from left to right, ie. 0.123 and 0.1234
	deque<unsigned char>::const_iterator it1 = this->m_Mantissa.get_data().begin();
	deque<unsigned char>::const_iterator it2 = a2.m_Mantissa.get_data().begin();
	for (; it1 != this->m_Mantissa.get_data().end() || it2 != a2.m_Mantissa.get_data().end();) {
		unsigned char uc1;
		unsigned char uc2;
		if (it1 == this->m_Mantissa.get_data().end()) {
			// it1 reached end of container, use 0 for uc1
			uc1 = 0;
		}
		else {
			uc1 = *it1;
			it1++;
		}
		if (it2 == a2.m_Mantissa.get_data().end()) {
			// it2 reached end of container, use 0 for uc2
			uc2 = 0;
		}
		else {
			uc2 = *it2;
			it2++;
		}
		// Compare corresponding positions
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

// Used when input is parsed into a number in CLexer::number() to form Mantissa
void CNumber::append_mantissa(int digit)
{
	this->m_Mantissa.tail_append(digit);
}

// Used when input is parsed into a scientific notation (ie. 1E+2) in CLexer::number() to form m_Exp
void CNumber::append_exponent(int digit)
{
	this->m_Exp.tail_append(digit);
}

// Used when input scientific notation is parsed ie. 1e+2 in CLexer::number() 
// to increment m_Exp using CBigInt::operator += ()
void CNumber::increment_exp(const CBigInt& exp)
{
	this->m_Exp += exp;
}

// Optimize CNumber respresentation removing leading and tailing zeroes
// Example:
//		Mantissa: 000123000
//		Exponent: 000321000
// Result:
//		Mantissa: 123
//		Exponent: 321000
void CNumber::remove_zeroes()
{
	this->m_Mantissa.remove_leading_zeroes();
	this->m_Mantissa.remove_tailing_zeroes();
	this->m_Zero = (this->m_Mantissa.length() == 1 && this->m_Mantissa.get_data()[0] == 0);
	this->m_Exp.remove_leading_zeroes();
}

// Operator output (friend of class CNumber):
//	This outputs numbers in two formats:
//  regular: -.000123 or 123.76
//	scientific notation: .1E-1234 or -123E+20000
//  scientific notation is used when abs(exponent) >= SN_THRESHOLD
//  in regular notation number of digits after dot is limited by BC_SCALE
//  BC_SCALE is 1024 and test.sh calls bc with scale=1024. Test.sh will fail if these constants are not equal 
//  Special case is zero
ostream& operator <<(ostream& os, const CNumber& num)
{
	if (num.isZero()) {
		os << 0;
		return os;
	}
	CNumber num1 = num;
	num1.remove_zeroes();
	if (!num1.m_positive) {
		os << "-";
	}
	if (!(CBigInt(0) < num1.m_Exp)) {
		// Negative exponent
		// This should be "0.".
		// "." is to match bc output
		os << "."; 
		if (num1.m_Exp.cmp_abs(CBigInt(static_cast<int>(CConstants::SN_THRESHOLD))) < 0) {
			// Abs(exponent) is relatively small(<SN_THRESHOLD). Print all zeroes after dot before mantissa
			int n = num1.m_Exp.toInt();
			os << string(n, '0');
			num1.m_Mantissa.set_ntoprint(static_cast<int>(CConstants::BC_SCALE) - n);
			os << num1.m_Mantissa;
		}
		else {
			// To not print many zeroes we use scientific notation
			// Example: .5E-5000000000
			os << num1.m_Mantissa << "E-" << num1.m_Exp;
		}
	}
	else {
		// Positive exponent.
		if (num1.m_Exp.cmp_abs(CBigInt(static_cast<int>(CConstants::SN_THRESHOLD))) < 0) {
			// Abs(exponent) is relatively small(<256). Print all zeroes after mantissa if any
			int n = num1.m_Exp.toInt();
			int pos = 0;
			bool seenDot = false;
			int printedafterdot = 0;
			for (auto x : num1.m_Mantissa.get_data()) {
				if (seenDot) {
					if (printedafterdot == static_cast<int>(CConstants::BC_SCALE)) {
						break;
					}
					printedafterdot++;
				}
				x += '0';
				os << x;
				pos++;
				if (pos == n && num1.m_Mantissa.length() > n) {
					// This is for the following case:
					//		Mantissa: 12345
					//		Exponent: 3
					//		  Output: 123.45 
					os << ".";
					seenDot = true;
					continue;
				}			
			}
			if (n > pos) {
				// This is for the following case:
				// Mantissa: 123
				// Exponent: 5
				// Output:   12300
				os << string(n - pos, '0');
			}
		}
		else {
			// To not print many zeroes we use scientific notation
			// Example: .5E+5000000000
			os << "." << num1.m_Mantissa << "E+" << num1.m_Exp;
		}
	}

	return os;
}