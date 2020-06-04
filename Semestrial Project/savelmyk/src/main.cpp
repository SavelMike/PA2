// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"
#include <string>

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

void admin_commands()
{
	string str;
	getline(cin, str);
	if (str == "quit") {
		exit(0);
	}
	if (str != "help" && str != "load" && str != "save") {
		cout << str << " - unknown command" << endl;
	}
	else {
		cout << str << " - command is not ready" << endl;
	}
}

bool set_var(CLexer& lex)
{
	string name;
	
	while (1) {
		char c = cin.get();
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
		cin.putback(c);
		for (int i = name.length() - 1; i >= 0; i--) {
			cin.putback(name[i]);
		}
		return false;
	}
}

int main()
{
	CLexer lex;

	while (1) {	
		cerr << "Enter expression or command:" << endl;
		try {
			char c;
		    if (!cin.get(c)) {
				// End of input stream
				break;
			}
			if (c == ':') {
				// :quit, :save, :load, :help
				admin_commands();
				continue;
			}
			if (isalpha(c)) {
				// Set variable
				cin.putback(c);
				if (set_var(lex)) {
					cin.get();
					continue;
				}
			}
			else {
				cin.putback(c);
			}
			
			// Execute expression
			cerr << "Result of expression: "; 
			cout << expr(lex) << endl;
			cin.get();	
		}
		catch (const char* str) {
			cout << str << endl;
			string s;
			getline(cin, s);
		}
	}
	return 0;
}
