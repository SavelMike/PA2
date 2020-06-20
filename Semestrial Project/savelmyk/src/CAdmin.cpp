#include "CAdmin.h"
#include "CLexer.h"
#include <fstream>

// Virtual method cmd for class CQuit
// Saves variables to file $HOME/.savelmyk_vars
// Set calc_over flag
// It is triggered via :quit
void CQuit::cmd(CLexer& lex)
{
	lex.save_variables();
	lex.calc_over();
}

// Virtual method cmd for class CHelp
// Outputs the help information
// It is triggered via :help
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

// Virtual method cmd for class CHistory
// List operations history which is saved in $HOME/.savelmyk_history
// Operations are numbered
// It is triggered via :history
void CHistory::cmd(CLexer& lex)
{
	ifstream history;
	history.open(lex.get_home_dir() + ".savelmyk_history");
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

// Virtual method cmd for class CClear
// Removes $HOME/.savelmyk_history
// It is triggered via :clear
void CClear::cmd(CLexer& lex)
{
	string filename = lex.get_home_dir() + ".savelmyk_history";
	remove(filename.c_str());
}

// Virtual method cmd for class CVariables
// Variables are in map<string, CNumber> lex.m_vars
// Output all variables in format "name = value"
// It is triggered via :variables
void CVariables::cmd(CLexer& lex)
{
	for (auto x : lex.get_m_vars()) {
		cout << x.first << " = " << x.second << endl;
	}
}

CNumber expr(CLexer& lex);

// Virtual method cmd for class CRepeat
// Read number of command to be repeated
// Find the requested command in $HOME/.savelmyk_history and calculates the expression
// It is triggered via :[1-9]+
void CRepeat::cmd(CLexer& lex)
{
	string str;
	getline(lex.get_input_stream(), str);
	int n = lex.get_int(str);
	if (n == 0) {
		return;
	}
	ifstream history;
	history.open(lex.get_home_dir() + ".savelmyk_history");
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
				// '=' is found, this is set var
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
