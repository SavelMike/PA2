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
	CNumber res;

	while (1) {
		// to do		
	}

	return res;
}

// Reads next char from input, return COperation corresponding to read char, CMul or CDef or CMod
COperation CLexer::factorop()
{
	m_last = cin.get();

	switch (m_last) {
	case '*':
		return CMul();
	case '/':
		return CDiv();
	case '%':
		return CMod();
	default:
		cin.putback(m_last);
	}
	return CNone();
}

// Reads next char from input, return COperation corresponding to read char, CAdd or CSub
COperation CLexer::exprop()
{
	m_last = cin.get();

	switch (m_last) {
	case '+':
		return CAdd();
	case '-':
		return CSub();
	case '\n':
		return CNone();
	default:
		cin.putback(m_last);
		return CNone();
	}
}
