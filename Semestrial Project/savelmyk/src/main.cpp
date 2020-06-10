// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"
#include <string>
#include <sstream>

using namespace std;

CNumber expr(CLexer& lex);

CNumber parenthesis(CLexer& lex)
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

CNumber factor(CLexer& lex)
{
	CNumber v1 = parenthesis(lex);

	while (1) {
		COperation* op = lex.factorop();
		if (op == nullptr) {
			// Nor multiply, division or remainder of division
			return v1;
		}
		CNumber v2 = parenthesis(lex);
		// The below will run CMul::operation or CDiv::operation or CMod::operation
		v1 = op->operation(&v1, &v2);
		delete op;
	}
	
	throw "Unexpected return from factor()";
}

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
		v1 = op->operation(&v1, &v2);
		delete op;
	}
	
	throw "Unexpected return from expr()";
}

void admin_commands(CLexer& lex)
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
}

bool set_var(CLexer& lex)
{
	string name;
	char c;
	
	while (1) {
		c = lex.get();
		if (!lex.get_input_stream()) {
			break;
		}
		if (isalpha(c) || isdigit(c)) {
			name += c;
			continue;
		}
		if (c == '=') {
			CNumber value = expr(lex);
			lex.add_variable(name, value);
			cerr << "var " << name << " = " << value << " is added" << endl;
			return true;
		}
		break;
	}
	if (lex.get_input_stream()) {
		lex.putback(c);
	}
	for (int i = name.length() - 1; i >= 0; i--) {
		lex.putback(name[i]);
	}
	return false;
}

int main()
{
	CLexer lex;

	while (1) {	
		cerr << "Enter expression or command:" << endl;
		string s;
		getline(cin, s);
		if (!cin) {
			break;
		}
		if (s.empty()) {
			continue;
		}
		lex.save_command(s);
		lex.set_input(s);
		try {
			char c = lex.get();
			if (!lex.get_input_stream()) {
				// End of input stream
				break;
			}
			if (c == ':') {
				// :quit, :save, :load, :help, :show history
				admin_commands(lex);
				continue;
			}
			if (isalpha(c)) {
				// Set variable
				lex.putback(c);
				if (set_var(lex)) {
					continue;
				}
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
