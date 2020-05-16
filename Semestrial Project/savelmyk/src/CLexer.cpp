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
	bool seenE = false;
	bool seenMinus = false;
	long long exp = 0;
	
	while (1) {
		char c = cin.get();
		if (c >= '0' && c <= '9') {
			if (seenE) {
				num.append_exponent(c - '0');
				continue;
			}
			num.append_mantissa(c - '0');
			if (!seenDot) {
				exp++;
			}
			num.set_valid();
			continue;
		}
		if (c == '.') {
			if (seenDot || seenE) {
				throw "Syntax error";
			}
			seenDot = true;
			continue;
		}
		if (c == 'E' || c == 'e') {
			if (seenE) {
				throw "Syntax error";
			}
			seenE = true;
			continue;
		}
		if (c == '-') {
			if (seenMinus) {
				throw "Syntax error";
			}
			seenMinus = true;
			if (!seenE) {
				cin.putback(c);
				num.increment_exp(exp);
				break;
			}
			if (num.exponent_length() != 0) {
				throw "Syntax error";
			}
			continue;
		}
		// Unacceptable char
		if (num.isinvalid()) {
			throw "Number is not found";
		}
		cin.putback(c);
		num.increment_exp(exp);
		break;
	}
	
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
	default:
		cin.putback(m_last);
		break;
	}
	return nullptr;
}
