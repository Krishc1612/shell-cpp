#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
using namespace std::filesystem;

pair<bool, string> hasValidExecutable(string& PATH, string& command){
	string envVar = "";

	for (int i = 0; i < PATH.size(); i++){
		if (PATH[i] == ':'){
			envVar += "/" + command;
			path p = envVar;

			if (exists(p) && is_regular_file(p)){
				auto perm = status(p).permissions();
				bool executable = ((perm & perms::owner_exec) != perms::none);

				if (executable) {
					return {true, envVar};
				}
			}

			envVar = "";
		}
		else envVar += PATH[i];
	}

	return {false, ""};
}

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	// TODO: Uncomment the code below to pass the first stage
	unordered_set<string> builtins = {
		"exit",
		"echo",
		"type",
		"pwd"
	};

	while (true){
		cout << "$ ";

		string input;
		getline(cin, input);

		if (input == "exit") {
			break;
		}
		else if (input.substr(0, 5) == "echo "){
			cout << input.substr(5, input.size()) << endl;
		}
		else if (input.substr(0, 5) == "type "){
			string command = input.substr(5);

			if (builtins.find(command) != builtins.end()){
				cout << command << " is a shell builtin" << endl;
				continue;
			}

			string PATH = getenv("PATH");
			PATH += ":";

			auto [isValid, pathTo] = hasValidExecutable(PATH, command);

			if (isValid){
				cout << command << " is " << pathTo << endl;
			}
			else {
				cout << command << ": not found" << endl;
			}
		}
		else if (input.substr(0, 4) == "pwd "){
			string pwdPath = "/usr/bin/pwd";

			if (fork() == 0){
				vector<char*> args;
				args.push_back(pwdPath.data());

				execv(pwdPath.data(), args.data());
			}
			else {
				wait(nullptr);
			}
		}
		else {
			vector<string> params;
			string arg;

			for (int i = 0; i < input.size(); i++){
				if (input[i] != ' ') arg += input[i];
				else {
					params.push_back(arg);
					arg = "";
				}
			}
			if (arg != "") params.push_back(arg);

			string command = params[0];

			string PATH = getenv("PATH");
			PATH += ":";

			auto [isValid, pathTo] = hasValidExecutable(PATH, command);

			if (isValid){
				if (fork() == 0){
					const char* p = pathTo.data();
					vector<char*> args;

					for (auto& param: params){
						args.push_back(param.data());
					}
					args.push_back(nullptr);

					execv(p, args.data());
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
