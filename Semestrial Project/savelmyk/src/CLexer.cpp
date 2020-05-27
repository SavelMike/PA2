// CLexer used to parse input into lexems

#include "CLexer.h"

// Used to try if next char is ( or ) 
bool CLexer::isparenthesis(char p)
{
	// Skip leading spaces
	while (1) {
		char c = cin.get();
		if (c == ' ') {
			continue;
		}
		else {
			m_last = c;
			break;
		}
	}
	if (m_last == p)
		return true;
	cin.putback(m_last);
	return false;
}

// Reads input char by char until input is valid decimal number, i.e.
//		123456789 -> mantissa: 123456789, exp 9
//		1234.56789 -> mantissa: 123456789, exp 4
//		1.234E56789 -> mantissa: 1234, exp 56790
// Store mantissa and exponent to member variables of CNumber

CNumber CLexer::number()
{	
	// Skip leading whitespaces 
	while (1) {
		char c = cin.get();
		if (c == ' ') {
			continue;
		}
		else {
			cin.putback(c);
			break;
		}
	}
	
	CNumber num;
	
	bool seenDot = false;
	bool zeroes_only = true;
	CBigInt exp(0);
	
	// Cycle for mantissa
	char c;
	while (1) {
	    c = cin.get();
		if (isdigit(c)) {
			num.append_mantissa(c - '0');
			num.set_valid();
			if (c == '0' && zeroes_only && seenDot) {
				exp -= CBigInt(1);
				continue;
			}
			if (c != '0') {
				zeroes_only = false;
			}
			if (!seenDot && !zeroes_only) {
				exp += CBigInt(1);
			}
			continue;
		}
		if (c == '.') {
			if (seenDot) {
				throw "Syntax error";
			}
			seenDot = true;
			continue;
		}
		break;
	}
	
	if (c != 'E' && c != 'e') {
		cin.putback(c);
		num.set_exponent(exp);
		num.remove_zeroes();

		return num;
	}

	c = cin.get();
	if (c == '-') {
		num.set_expsign(false);
	} else if (c == '+') {
		num.set_expsign(true);
	} else if (!isdigit(c)) {
		throw "Syntax error";
	} else {
		num.append_exponent(c - '0');
	}
	
	while (1) {
		c = cin.get();
		if (isdigit(c)) {
			num.append_exponent(c - '0');
			continue;
		}
		cin.putback(c);
		break;
	}

	num.increment_exp(exp);
	num.remove_zeroes();
	
	return num;
}

// Reads next char from input, return COperation corresponding to read char, CMul or CDef or CMod
COperation* CLexer::factorop()
{
	// Skip leading spaces
	while (1) {
		char c = cin.get();
		if (c == ' ') {
			continue;
		}
		else {
			m_last = c;
			break;
		}
	}

	switch (m_last) {
	case '*':
		return new CMul();
	case '/':
		return new CDiv();
	case '%':
		return new CMod();
	default:
		cin.putback(m_last);
		break;
	}
	return nullptr;
}

// Reads next char from input, return COperation corresponding to read char, CAdd or CSub
COperation* CLexer::exprop()
{
	// Skip leading spaces
	while (1) {
		char c = cin.get();
		if (c == ' ') {
			continue;
		}
		else {
			m_last = c;
			break;
		}
	}

	switch (m_last) {
	case '+':
		return new CAdd();
	case '-':
		return new CSub();
	case '\n':
		break;
	default:
		cin.putback(m_last);
//		throw "syntax error";
		break;
	}
	return nullptr;
}
