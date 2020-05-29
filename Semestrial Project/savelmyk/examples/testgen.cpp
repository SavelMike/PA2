#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(void)
{
	string str;
	vector <string> vec;
	vector <char> ops = {'+' , '-' , '+'};

	while (cin >> str) {
		vec.push_back(str);
	}

	for (auto o : ops) {
		for (auto x : vec) {
			for (auto y : vec) {
				cout << x << o << y << endl;
			}
		}
	}

	return 0;
}
