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

// Reads input char by char until input is valid decimal number, i.e. 123.456E789
// Store mantissa and exponent to member variables of CNumber
CNumber CLexer::number()
{
	int res = 0;
	
	// Skip leading whitespaces 
	while(1) {
		char c = cin.get();
		if (c == ' ') {
			continue;
		}
		else {
			cin.putback(c);
			break;
		}
	}
	
	int ndigits = 0;
	while(1) {
		char c = cin.get();
		if (c >= '0' && c <= '9') {
			res = res * 10 + c - '0';
			ndigits++;
		}
		else
		{
			if (ndigits == 0) {
				throw "Number is not found";
			}
			cin.putback(c);
			break;
		}
	}
	return CNumber(res);
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
