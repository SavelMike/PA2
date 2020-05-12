// CLexer used to parse input into lexems

#include "CLexer.h"

// Used to try if next char is ( or ) 
bool CLexer::isparenthesis(char p)
{
	m_last = cin.get();
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
	
	while(1) {
		char c = cin.get();
		if (c >= '0' && c <= '9') {
			res = res * 10 + c - '0';
		}
		else
		{
			cin.putback(c);
			break;
		}
	}
	return CNumber(res);
}

// Reads next char from input, return COperation corresponding to read char, CMul or CDef or CMod
COperation* CLexer::factorop()
{
	m_last = cin.get();

	switch (m_last) {
	case '*':
		return new CMul();
	case '/':
		return new CDiv();
	case '%':
		return new CMod();
	default:
		cin.putback(m_last);
	}
	return new CNone();
}

// Reads next char from input, return COperation corresponding to read char, CAdd or CSub
COperation* CLexer::exprop()
{
	m_last = cin.get();

	switch (m_last) {
	case '+':
		return new CAdd();
	case '-':
		return new CSub();
	case '\n':
		return new CNone();
	default:
		cin.putback(m_last);
		return new CNone();
	}
}
