// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"
#include <string>

CNumber expr(CLexer& lex);

CNumber factor(CLexer& lex)
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

	COperation* op = lex.factorop();
	if (op == nullptr) {
		// Nor multiply, division or remainder of division
		return v1;
	}
	CNumber v2 = factor(lex);

	// The below will run CMul::operation or CDiv::operation or CMod::operation
	CNumber res = op->operation(&v1, &v2);
	delete op;
	
	return res;
}

CNumber expr(CLexer& lex)
{
	CNumber v1 = factor(lex);
	COperation* op = lex.exprop();
	if (op == nullptr) {
		// Op is not plus and not minus
		return v1;
	}
	CNumber v2 = expr(lex);
	
	// The below will run CAdd::operation or CSub::operation 
	CNumber res = op->operation(&v1, &v2);
	delete op;
	
	return res;
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

void set_var()
{
	string str;
	getline(cin, str);
	cout << "set_var is not ready" << endl;
	// cin.get(); // Get '\n'
}

int main()
{
	CLexer lex;
	
	while (!cin.eof()) {
		
		cerr << "Enter expression or command:" << endl;
		try {
			char c = cin.get();
			if (c == ':') {
				// :quit, :save, :load, :help
				admin_commands();
			}
			else if (isalpha(c)) {
				// Set variable
				cin.putback(c);
				set_var();
			}
			else {
				// Execute expression
				cin.putback(c);
				cerr << "Result of expression: "; 
				cout << expr(lex) << endl;
			}
		}
		catch (const char* str) {
			cout << str << endl;
			string s;
			getline(cin, s);
		}
	}
	return 0;
}
