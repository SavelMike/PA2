// CLexer used to parse input into lexems

#include "CLexer.h"
#include "fstream"

// CLexer default constructor
//		calculates home directory using environment variables
//		loads variable from file $HOME/.savelmyk_vars 
CLexer::CLexer() :m_parenths(0), m_calc_over(false)
{
#ifdef _WIN32
	m_homedir = getenv("HOMEDRIVE");
	m_homedir += getenv("HOMEPATH");
	m_homedir += '\\';
#else
	m_homedir = getenv("HOME");
	m_homedir += '/';
#endif
	this->load_variables();
}

// Used to try if next char is '(' or ')'
//	arguments: 
//		p: '(' or ')' 
//  return: 
//		true if next non-whitespace char == p
//		false otherwise
bool CLexer::isparenthesis(char p)
{
	char c;
	
	// Skip leading spaces and tabs
	while (1) {
		c = this->get();
		if (c == EOF) {
			return false;
		}
		if (c == ' ' || c == '\t') {
			continue;
		}
		break;
	}
	if (c == p) {
		// Count parenthesis
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
//		123456789 -> mantissa: 123456789, exp: 9
//		1234.56789 -> mantissa: 123456789, exp: 4
//		1.234E56789 -> mantissa: 1234, exp: 56790
//		0.0001e-10 -> mantissa: 1, exp: -13
// Store mantissa and exponent to member variables of CNumber
// If input starts with alpha, assume that this is variable name.
// Look for variable in map and return its value if found
// Return value:
//		CNumber::parsed number or value of variable
CNumber CLexer::number()
{	
	char c;
	
	// Skip leading spaces and tabs
	while (1) {
		c = this->get();
		if (c == ' ' || c == '\t') {
			continue;
		}
		break;
	}
	
	CNumber num;
	
	// Try name of variable
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
				// One more zero after dot
				// ie. if input is 0.000 then exp == -3
				exp -= CBigInt(1);
				continue;
			}
			if (c != '0' && zeroes_only) {
				// Leading zeroes are over
				// ie. 0001 or 0.0002
				zeroes_only = false;
			}
			if (!seenDot && !zeroes_only) {
				// No dot so far
				// ie. if input is 123 then exp == 3
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
		// Not a scientific notation, num is almost ready
		if (c != EOF) {
			this->putback(c);
		}
		num.set_exponent(exp);
		num.remove_zeroes();

		return num;
	}

	// Parse scientific notation, ie. e+125 or E-15
	bool gotDigit = false;
	c = this->get();
	if (c == '-') {
		num.set_expsign(false);
	} else if (c == '+') {
		num.set_expsign(true);
	} else if (!isdigit(c)) {
		throw "Syntax error";
	} else {
		num.append_exponent(c - '0');
		gotDigit = true;
	}
	
	// Read exponent digit by digit
	while (1) {
		c = this->get();
		if (c == EOF) {
			break;
		}
		if (isdigit(c)) {
			num.append_exponent(c - '0');
			gotDigit = true;
			continue;
		}
		this->putback(c);
		break;
	}

	if (!gotDigit) {
		// No digit after e or E
		// ie. 1.23e-
		throw "Syntax error";
	}
	num.increment_exp(exp);
	num.remove_zeroes();

	return num;
}

// Reads next char from input, 
//		return COperation* corresponding to read char: 
//		'*' : CMul* 
//		'/' : CDiv*
//		'%' : CMod*
//		return nullptr if '+', '-', '\n' or ')' is read
//		throw exception if any other symbol is read
COperation* CLexer::factorop()
{
	char c;
	
	// Skip leading spaces and tabs
	while (1) {
		c = this->get();
		if (c == EOF) {
			return nullptr;
		}
		if (c == ' ' || c == '\t') {
			continue;
		}
		break;
	}

	switch (c) {
	case '*':
		return new CMul();
	case '/':
		return new CDiv();
	case '%':
		return new CMod();
	case ')':
		// ')' is ok, only if there were open paranthesis 
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

// Reads next char from input
//		return COperation* corresponding to read char: 
//		'+' : CAdd* 
//		'-' : CSub*
//		return nullptr if '*', '/', '%', '\n' or ')' is read
//		throw exception if any other symbol is read
COperation* CLexer::exprop()
{
	char c;

	// Skip leading spaces and tabs
	while (1) {
		c = this->get();
		if (c == EOF) {
			return nullptr;
		}
		if (c == ' ' || c == '\t') {
			continue;
		}
		break;
	}

	switch (c) {
	case '+':
		return new CAdd();
	case '-':
		return new CSub();
	case ')':
		// ')' is ok, only if there were open paranthesis 
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

// It is called when input stream starts with ':'
// Read the rest of input string stream
//		return CAdmin* corresponding to read string: 
//		"quit" : CQuit* 
//		"help" : CHelp*
//		"history" : CHistory*
//		"clear" : CClear*
//		"variables" : CVariables*
//		[0-9] : CRepeat*
//		return nullptr otherwise
CAdmin* CLexer::adminop()
{
	string str;
	char c = this->get();
		
	switch (c)
	{
	default:
		break;
	case 'q':
		getline(this->get_input_stream(), str);
		if (str == "uit") {
			return new CQuit();
		}
		break;
	case 'h':
		getline(this->get_input_stream(), str);
		if (str == "elp") {
			return new CHelp();
		}
		if (str == "istory") {
			return new CHistory();
		}
		break;
	case 'c':
		getline(this->get_input_stream(), str);
		if (str == "lear") {
			return new CClear();
		}
		break;
	case 'v':
		getline(this->get_input_stream(), str);
		if (str == "ariables") {
			return new CVariables();
		}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		this->putback(c);
		return new CRepeat();
	}
	return nullptr;
}

// This is to save variable name and variable value in m_vars container
void CLexer::add_variable(const string& name, const CNumber& value)
{
	this->m_vars[name] = value;
}

// This is called automatically on every command, 
// it appends the string str to $HOME/.savelmyk_history
void CLexer::save_command(const string& str)
{
	ofstream history;
	history.open(m_homedir + ".savelmyk_history", ios_base::app);
	history << str << endl;
	history.close();
}

// This is part of CLexer constructor, opens $HOME/.savelmyk_vars and builds m_vars map
void CLexer::load_variables()
{
	ifstream vars;
	vars.open(m_homedir + ".savelmyk_vars");
	if (!vars.is_open()) {
		return;
	}
	string str;
	while (getline(vars, str)) {
		this->set_input(str);
		string name;
		// Str is in format "a=3.5"
		while (1) {
			char c = this->get();
			if (c == EOF) {
				break;
			}
			if (isalpha(c) || isdigit(c)) {
				name += c;
				continue;
			}
			// Name is ready
			if (c == '=') {
				bool sign = true;
				c = this->get();
				if (c == '-') {
					sign = false;
				} else {
					this->putback(c);
				}
				CNumber value = this->number();
				value.set_sign(sign);
				this->add_variable(name, value);
			}
			break;
		}
	}
	vars.close();
}

// This runs on program termiantion
// Iterate over m_vars container and output variable name 
// and values to file $HOME/.savelmyk_vars
// in format: myvar=45
// old content of the file is removed
void CLexer::save_variables()
{
	ofstream vars;
	vars.open(m_homedir + ".savelmyk_vars", ios_base::trunc);
	for (auto x : this->m_vars) {
		vars << x.first << "=" << x.second << endl;
	}
	vars.close();
	m_vars.clear();
}

// Used in CRepeat::cmd()
// Convert str to integer and return it
// 0 is returned when there is not a digit in str
int CLexer::get_int(const string& str)
{
	int res = 0;
	for (auto x: str) {
		if (isdigit(x)) {
			res = 10 * res + x - '0';
			continue;
		}
		return 0;
	}

	return res;
}