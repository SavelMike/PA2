// CLexer used to parse input into lexems
#include "CNumber.h"
#include "COperation.h"
#include <map>
#include <sstream>

class CLexer {
public:
	CLexer() :m_parenths(0) { ; }
	bool isparenthesis(char p);
	void putback();
	CNumber number();
	COperation* factorop();
	COperation* exprop();
	void add_variable(const string&, const CNumber&);
	void set_input(const string& s) { m_sin.clear(); m_sin.str(s); m_parenths = 0; }
	char get() { return m_sin.get(); }
	void putback(char c) { m_sin.putback(c); }
	istringstream& get_input_stream() { return m_sin; }
	void save_command(const string& str);
private:
	map<string, CNumber> m_vars;
	istringstream m_sin;
	int m_parenths; // Number of open parenthesis
};