// Arbitrary precision calculator

#include "CNumber.h"
#include "COperation.h"
#include "CLexer.h"

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
	if (!(op->isFactor())) {
		// Nor multiply, division or remainder of division
		return v1;
	}
	CNumber v2 = factor(lex);

	// The below will run CMul::operation or CDiv::operation or CMod::operation
	return op->operation(&v1, &v2);
}

CNumber expr(CLexer& lex)
{
	CNumber v1 = factor(lex);
	COperation* op = lex.exprop();
	if (!(op->isExpr())) {
		// Op is not plus and not minus
		return v1;
	}
	CNumber v2 = expr(lex);
	
	// The below will run CAdd::operation or CSub::operation 
	return op->operation(&v1, &v2);
}

int main()
{
	CLexer lex;
	
	cout << "enter expression: " << endl;
	cout << expr(lex) << endl;
	return 0;
}
