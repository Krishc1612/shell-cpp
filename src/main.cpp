#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"
#include "path_utils.h"

using namespace std;
using namespace std::filesystem;

unordered_map<string, function<void(vector<string>)>> commands = {
	{"exit", shellExit},
	{"echo", echo},
	{"type", type},
	{"pwd", pwd},
	{"cd", cd}
};

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
			if (doubleQuote || singleQuote){
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

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	while (true){
		cout << "$ ";

		string input;
		getline(cin, input);

		vector<string> args = tokenize(input);

		if (args[0] == "exit"){
			break;
		}
		else if (commands.find(args[0]) != commands.end()){
			commands[args[0]](args);
		}
		else {
			string command = args[0];

			string PATH = getenv("PATH");
			PATH += ":";

			auto [isValid, pathTo] = hasValidExecutable(PATH, command);

			if (isValid){
				if (fork() == 0){
					const char* p = pathTo.data();
					vector<char*> execArgs;

					for (auto& arg: args){
						execArgs.push_back(arg.data());
					}
					execArgs.push_back(nullptr);

					execv(p, execArgs.data());
				}
				else {
					wait(nullptr);
				}
			}
			else {
				cout << command << ": not found" << endl;
			}
		}
	}

	return 0;
}