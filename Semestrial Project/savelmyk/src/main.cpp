// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"
#include <string>
#include <sstream>

using namespace std;

CNumber expr(CLexer& lex);

// This parses next operand from input stream
// Example of operand:
//   (5 + 10 * 2)
//   10e-700
// Return: value of next operand in input stream
// Arguments:
//	 lex - Clexer object. It contains expression to execute in istringstream(m_sin).
CNumber operand(CLexer& lex)
{
	CNumber v1;
	
	if (lex.isparenthesis('(')) {
		v1 = expr(lex);
		if (!lex.isparenthesis(')')) {
			throw("missing closing parenthesis");
		}
	}
	else {
		v1 = lex.number();
	}
	return v1;
}

// This function executes expression consisting of *, / and % from left to right
// Example: 10 * 25 / (7 + 2) * 2.5e-16
// Return: result of expression
// Arguments:
//	lex - Clexer object. It contains expression to execute in istringstream(m_sin).
CNumber factor(CLexer& lex)
{
	CNumber v1 = operand(lex);

	while (1) {
		COperation* op = lex.factorop();
		if (op == nullptr) {
			// Nor multiply, division or remainder of division
			return v1;
		}
		CNumber v2 = operand(lex);
		// The below will run CMul::operation or CDiv::operation or CMod::operation
		const char* exception = nullptr;
		// catch exception to be able to delete op
		try {
			v1 = op->operation(&v1, &v2);
		}
		catch (const char* str) {
			exception = str;
		}
		delete op;
		if (exception) {
			throw exception;
		}
	}
	
	throw "Unexpected return from factor()";
}

// This function executes expression consisting of additions and subtractions from left to right
// Example: 10 + 25 * 7 - 2.5e-16
// Return: result of expression
// Arguments:
//	lex - Clexer object. It contains expression to execute in istringstream(m_sin).
CNumber expr(CLexer& lex)
{
	CNumber v1 = factor(lex);
	
	while (1) {
		COperation* op = lex.exprop();
		if (op == nullptr) {
			// Op is not plus and not minus
			return v1;
		}
		CNumber v2 = factor(lex);

		// The below will run CAdd::operation or CSub::operation 
		const char* exception = nullptr;
		// catch exception to be able to delete op
		try {
			v1 = op->operation(&v1, &v2);
		}
		catch (const char* str) {
			exception = str;
		}
		delete op;
		if (exception) {
			throw exception;
		}
	}
	
	throw "Unexpected return from expr()";
}

// This is called ':' is read from input stream
// Then it parses and executes admin command
// Return: boolean, true if :quit was handled
// Arguments:
//	 lex - Clexer object. It contains expression to execute in istringstream(m_sin).
bool admin_commands(CLexer& lex)
{
	CAdmin* adm = lex.adminop();
	if (adm != nullptr) {
		// adm is CHelp*, CHistory*, ... 
		adm->cmd(lex);
		delete adm;
	}
	else {
		cout << "Unknown command" << endl;
	}
	return lex.is_calc_over();
}

// This parses strings like "Pi=3.14"
// This is called when input string stream starts with a letter
// Variable and its value is added to lex.m_variable(map<string, CNumber>)
// Example:
//   sq=Pi*r*r/2
//	 myvar123=7
//	 ppp			- wrong format, false will be returned
//   foovar=(5+))   - exception will be thrown
// Return: true if variable is added, false if input doesn't look like Pi=3.14
bool set_var(CLexer& lex)
{
	string name;
	char c;
	
	while (1) {
		c = lex.get();
		if (c == EOF) {
			break;
		}
		if (isalpha(c) || isdigit(c)) {
			//  Append the char c to name
			name += c;
			continue;
		}
		if (c == '=') {
			// Name of variable is read, calculate its value
			CNumber value = expr(lex);
			lex.add_variable(name, value);
			return true;
		}
		// This is not set variable command
		break;
	}
	return false;
}

int main()
{
	CLexer lex;

	while (1) {	
		cerr << "Enter expression or command:" << endl;
		// Get next line from input
		string s;
		getline(cin, s);
		if (!cin) {
			// EOF is met
			break;
		}
		if (s.empty()) {
			// Empty string is met
			continue;
		}
		lex.save_command(s); // Record command to file $HOME/.savelmyk_history
		lex.set_input(s); // Associate Lexer input string stream with string s
		try {
			char c = lex.get();
			if (c == EOF) {
				break;
			}
			if (c == ':') {
				// :quit, :history, :clear, :help, :variables, :num
				if (admin_commands(lex)) {
					// :quit was entered
					break;
				}
				continue;
			}
			if (isalpha(c)) {
				// Try set variable
				lex.putback(c);
				if (set_var(lex)) {
					// Variable and its value is ok 
					continue;
				}
				// Reset input stream to execute expression
				lex.set_input(s);
			}
			else {
				lex.putback(c);
			}
			
			// Execute expression
			cerr << "Result of expression: "; 
			cout << expr(lex) << endl;
		}
		catch (const char* str) {
			cout << str << endl;
		}
	}
	lex.save_variables();
	return 0;
}
