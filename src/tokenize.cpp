#include "tokenize.h"

vector<TokenType> fileDesc = {REDIRECT_IN, REDIRECT_OUT, REDIRECT_ERR};

vector<Token> tokenize(string& input){
	vector<Token> args;
	Token arg;

	bool singleQuote = false;
	bool doubleQuote = false;
	bool isSlashed = false;

	for (const auto& c : input){
		if (isSlashed){
			arg.data += c;
			isSlashed = false;
			continue;
		}

		if (c == '>' && (!doubleQuote && !singleQuote)){
			if (arg.data.empty()){
				arg.data += c;
				arg.type = REDIRECT_OUT;
			}
			else if (
				arg.data.size() == 1 && 
				(arg.data[0] >= '0' && arg.data[0] <= '2') 
			){
				arg.data += c;
				arg.type = fileDesc[arg.data[0] - '0'];
			}
			else {
				args.push_back(arg);
				arg.data = string(1, c);
				arg.type = REDIRECT_OUT;
			}

			args.push_back(arg);
			arg.data = "";
			arg.type = WORD;
		}
		else if (c == '\"' && !singleQuote){
			doubleQuote = !doubleQuote;
		}
		else if (c == '\'' && !doubleQuote){
			singleQuote = !singleQuote;
		}
		else {
			if (doubleQuote){
				if (c == '\\') isSlashed = true;
				else arg.data += c;
			}
			else if (singleQuote){
				arg.data += c;
			}
			else {
				if (c != ' ') {
					if (c == '\\') isSlashed = true;
					else arg.data += c;
				}
				else if (c == ' ' && !arg.data.empty()) {
					args.push_back(arg);
					arg.data = "";
					arg.type = WORD;
				}
			}
		}
	}
	if (!arg.data.empty()) args.push_back(arg);

	return args;
}