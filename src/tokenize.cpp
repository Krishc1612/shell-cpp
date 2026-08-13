#include "tokenize.h"

vector<string> tokenize(string& input){
	vector<string> args;
	string arg = "";

	bool singleQuote = false;
	bool doubleQuote = false;
	bool isSlashed = false;

	for (const auto& c : input){
		if (isSlashed){
			arg += c;
			isSlashed = false;
			continue;
		}

		if (c == '\"' && !singleQuote){
			doubleQuote = !doubleQuote;
		}
		else if (c == '\'' && !doubleQuote){
			singleQuote = !singleQuote;
		}
		else {
			if (doubleQuote){
				if (c == '\\') isSlashed = true;
				else arg += c;
			}
			else if (singleQuote){
				arg += c;
			}
			else {
				if (c != ' ') {
					if (c == '\\') isSlashed = true;
					else arg += c;
				}
				else if (c == ' ' && !arg.empty()) {
					args.push_back(arg);
					arg = "";
				}
			}
		}
	}
	if (!arg.empty()) args.push_back(arg);

	return args;
}