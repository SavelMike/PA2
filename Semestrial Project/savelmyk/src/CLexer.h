// CLexer used to parse input into lexems
#include "CNumber.h"
#include "COperation.h"
#include "CAdmin.h"
#include <map>
#include <sstream>

class CLexer {
public:
	CLexer();
	bool isparenthesis(char p);
	void putback();
	CNumber number();
	COperation* factorop();
	COperation* exprop();
	CAdmin* adminop();
	void add_variable(const string&, const CNumber&);
	void set_input(const string& s) { m_sin.clear(); m_sin.str(s); m_parenths = 0; }
	char get() { return m_sin.get(); }
	void putback(char c) { m_sin.putback(c); }
	istringstream& get_input_stream() { return m_sin; }
	map<string, CNumber>& get_m_vars() { return m_vars; }
	void save_command(const string& str);
	void load_variables();
	void save_variables();
	int get_int(const string& str);
	const string& get_home_dir() const { return m_homedir; }
	void calc_over() { m_calc_over = true; }
	bool is_calc_over() { return m_calc_over; }
private:
	map<string, CNumber> m_vars;
	istringstream m_sin;
	int m_parenths; // Number of open parenthesis
	string m_homedir;
	bool m_calc_over;
};