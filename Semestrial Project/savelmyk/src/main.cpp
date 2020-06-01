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
		CNumber v2 = expr(lex);

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
/*
	cout << "2 < 3 = " << ((CBigInt(2) < CBigInt(3)) ? "true" : "false") << endl;
	cout << "-2 < -3 = " << ((CBigInt(-2) < CBigInt(-3)) ? "true" : "false") << endl;
	cout << "2 < -3 = " << ((CBigInt(2) < CBigInt(-3)) ? "true" : "false") << endl;
	cout << "-2 < 3 = " << ((CBigInt(-2) < CBigInt(3)) ? "true" : "false") << endl;
*/	
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
				cin.get();
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
