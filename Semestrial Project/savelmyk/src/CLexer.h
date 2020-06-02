// CLexer used to parse input into lexems
#include "CNumber.h"
#include "COperation.h"
#include <map>

class CLexer {
public:
	bool isparenthesis(char p);
	void putback();
	CNumber number();
	COperation* factorop();
	COperation* exprop();
	void add_variable(const string&, const CNumber&);
private:
	map<string, CNumber> m_vars;
};