// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"

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

	CNumber v2 = factor(lex);

	// The below will run CMul::operation or CDif::operation or CMod::operation
	return lex.factorop().operation(&v1, &v2);
}

CNumber expr(CLexer& lex)
{
	CNumber v1 = factor(lex);
	CNumber v2 = expr(lex);
	
	// The below will run CAdd::operation or CSub::operation 
	return lex.exprop().op(&v1, &v2);
}

int main()
{
	CLexer lex;
	
	cout << "enter expression: " << endl;
	cout << expr(lex) << endl;
	return 0;
}