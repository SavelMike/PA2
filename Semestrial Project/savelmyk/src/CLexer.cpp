// CLexer used to parse input into lexems

#include "CLexer.h"
#include "fstream"

// Used to try if next char is ( or ) 
bool CLexer::isparenthesis(char p)
{
	// Skip leading spaces
	char c;
	while (1) {
		c = this->get();
		if (!this->get_input_stream()) {
			return false;
		}
		if (c == ' ') {
			continue;
		}
		else {
			break;
		}
	}
	if (c == p) {
		if (p == '(') {
			m_parenths++;
		}
		else {
			m_parenths--;
		}
		return true;
	}
	this->putback(c);
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
		char c = this->get();
		if (c == ' ') {
			continue;
		}
		else {
			this->putback(c);
			break;
		}
	}
	
	CNumber num;
	
	// Try name of variable
	char c = this->get();
	if (isalpha(c)) {
		string name(1, c);
		while(1) {
			c = this->get();
			if (isdigit(c) || isalpha(c)) {
				name += c;
				continue;
			}
			// c cannot be in variable name
			this->putback(c);
			// Look for variable
			map<string, CNumber>::const_iterator it;
			it = m_vars.find(name);
			if (it == m_vars.end()) {
				// Variable name was not found
				throw "Variable not found";
			}
			return m_vars[name];
		}
	}
	// Parsing number
	this->putback(c);
	bool seenDot = false;
	bool zeroes_only = true;
	CBigInt exp(0);
	
	// Cycle for mantissa
	while (1) {
	    c = this->get();
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
		if (this->get_input_stream()) {
			this->putback(c);
		}
		num.set_exponent(exp);
		num.remove_zeroes();

		return num;
	}

	c = this->get();
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
		c = this->get();
		if (!this->get_input_stream()) {
			break;
		}
		if (isdigit(c)) {
			num.append_exponent(c - '0');
			continue;
		}
		this->putback(c);
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
	char c;
	
	while (1) {
		c = this->get();
		if (!this->get_input_stream()) {
			return nullptr;
		}
		if (c == ' ') {
			continue;
		}
		else {
			break;
		}
	}

	switch (c) {
	case '*':
		return new CMul();
	case '/':
		return new CDiv();
	case '%':
		return new CMod();
	case ')':
		if (m_parenths == 0) {
			throw "Syntax error";
		}
	case '+':
	case '-':
	case '\n':
		this->putback(c);
		break;
	default:
		throw "Syntax error";
	}
	return nullptr;
}

// Reads next char from input, return COperation corresponding to read char, CAdd or CSub
COperation* CLexer::exprop()
{
	// Skip leading spaces
	char c;

	while (1) {
		c = this->get();
		if (!this->get_input_stream()) {
			return nullptr;
		}
		if (c == ' ') {
			continue;
		}
		else {
			break;
		}
	}

	switch (c) {
	case '+':
		return new CAdd();
	case '-':
		return new CSub();
	case ')':
		if (m_parenths == 0) {
			throw "syntax error";
		}
	case '*':
	case '/':
	case '%':
	case '\n':
		this->putback(c);
		break;
	default:
		throw "syntax error";
	}
	return nullptr;
}

void CLexer::add_variable(const string& name, const CNumber& value)
{
	this->m_vars[name] = value;
}

void CLexer::save_command(const string& str)
{
	ofstream history; 
	history.open(".savelmyk_history", ios_base::app);
	history << str << endl;
	history.close();
}
