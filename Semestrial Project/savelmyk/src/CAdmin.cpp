#include "CAdmin.h"
#include "CLexer.h"
#include <fstream>

void CQuit::cmd(CLexer& lex)
{
	lex.save_variables();
	exit(0);
}

void CHelp::cmd(CLexer& lex)
{
	cout << endl;
	cout << "Description of the commands:" << endl;
	cout << "	:quit" << endl;
	cout << "	:help" << endl;
	cout << "	:history - displays list of recent commands entered by the user." << endl;
	cout << "	:clear - deletes list of commands." << endl;
	cout << "	:variables - displays list of variables and their values." << endl;
}

void CHistory::cmd(CLexer& lex)
{
	ifstream history;
	history.open(".savelmyk_history");
	if (!history.is_open()) {
		throw "Cannot open history file";
	}
	string str;
	int count = 0;
	while (getline(history, str)) {
		count++;
		cout << count << ") " << str << endl;
	}
	history.close();
}

void CClear::cmd(CLexer& lex)
{
	remove(".savelmyk_history");
}

void CVariables::cmd(CLexer& lex)
{
	for (auto x : lex.get_m_vars()) {
		cout << x.first << " = " << x.second << endl;
	}
}

CNumber expr(CLexer& lex);

void CRepeat::cmd(CLexer& lex)
{
	string str;
	getline(lex.get_input_stream(), str);
	int n = lex.get_int(str);
	if (n == 0) {
		return;
	}
	ifstream history;
	history.open(".savelmyk_history");
	int count = 0;
	while (getline(history, str)) {
		count++;
		if (n == count) {
			lex.set_input(str);
			char c = lex.get();
			if (c == ':') {
				cerr << "Don't repeat admin command" << endl;
				break;
			}
			lex.putback(c);
			if (str.find('=') != string::npos) {
				// = is found, this is set var
				cerr << "Don't repeat set_var" << endl;
				break;
			}
			// Execute expression
			cerr << "Result of expression: ";
			cout << expr(lex) << endl;
			break;
		}
	}
	history.close();
}
