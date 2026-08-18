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
#include "tokenize.h"
#include "parse.h"
#include "execute.h"

using namespace std;
using namespace std::filesystem;

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	while (true){
		cout << "$ ";

		string input;
		getline(cin, input);

		vector<Token> tokens = tokenize(input);

		// for (Token& token : tokens){
		// 	cout << "\"" << token.data << "\"" << " ";
		// }
		// cout << endl;

		if (tokens.size()){
			Task toExec = parse(tokens);
			bool isExit = execute(toExec);

			if (isExit) break;
		}

		// if (args[0] == "exit"){
		// 	break;
		// }
		// else if (commands.find(args[0]) != commands.end()){
		// 	commands[args[0]](args);
		// }
		// else {
		// 	string command = args[0];

		// 	string PATH = getenv("PATH");
		// 	PATH += ":";

		// 	auto [isValid, pathTo] = hasValidExecutable(PATH, command);

		// 	if (isValid){
		// 		if (fork() == 0){
		// 			const char* p = pathTo.data();
		// 			vector<char*> execArgs;

		// 			for (auto& arg: args){
		// 				execArgs.push_back(arg.data());
		// 			}
		// 			execArgs.push_back(nullptr);

		// 			execv(p, execArgs.data());
		// 		}
		// 		else {
		// 			wait(nullptr);
		// 		}
		// 	}
		// 	else {
		// 		cout << command << ": not found" << endl;
		// 	}
		// }
	}

	return 0;
}