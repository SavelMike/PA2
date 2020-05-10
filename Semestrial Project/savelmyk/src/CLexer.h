// CLexer used to parse input into lexems
#include "CNumber.h"
#include "COperation.h"

class CLexer {
public:
	bool isparenthesis(char p);
	void putback();
	CNumber number();
	COperation factorop();
	COperation exprop();
private:
	char m_last;
};