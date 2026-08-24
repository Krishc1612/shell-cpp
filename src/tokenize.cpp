#include "tokenize.h"

unordered_map<string, TokenType> fileDesc = {
	{"0>", REDIRECT_IN},
	{">", REDIRECT_OUT},
	{"1>", REDIRECT_OUT},
	{"2>", REDIRECT_ERR},
	{">>", REDIRECT_OUT_APP},
	{"1>>", REDIRECT_OUT_APP},
	{"2>>", REDIRECT_ERR_APP}
};

vector<Token> tokenize(string& input){
	vector<Token> args;
	Token arg;

	bool singleQuote = false;
	bool doubleQuote = false;
	bool isSlashed = false;
	bool isRedirect = false;

	for (const auto& c : input){
		if (isSlashed){
			arg.data += c;
			isSlashed = false;
			continue;
		}
		else if (isRedirect){
			isRedirect = false;

			if (c == '>' || c == ' '){
				if (c == '>') arg.data += c;
				arg.type = fileDesc[arg.data];

				args.push_back(arg);
				arg.data = "";
				arg.type = WORD;

				continue;
			}
		}

		if (c == '>' && (!doubleQuote && !singleQuote)){
			if (arg.data.empty()){
				arg.data += c;
			}
			else if (
				arg.data.size() == 1 && 
				(arg.data[0] >= '0' && arg.data[0] <= '2') 
			){
				arg.data += c;
			}
			else {
				args.push_back(arg);
				arg.data = string(1, c);
			}

			isRedirect = true;
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