#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	// TODO: Uncomment the code below to pass the first stage
	unordered_set<string> builtins = {
		"exit",
		"echo",
		"type"
	};

	while (true){
		cout << "$ ";

		string input;
		getline(cin, input);

		if (input == "exit") break;

		if (input.substr(0, 5) == "echo "){
			cout << input.substr(5, input.size()) << endl;
			continue;
		}

		if (input.substr(0, 5) == "type "){
			string command = input.substr(5);

			if (builtins.find(command) != builtins.end()){
				cout << command << " is a shell builtin" << endl;
				continue;
			}

			string PATH = getenv("PATH");
			PATH += ":";

			string envVar = "";
			bool found = false;

			for (int i = 0; i < PATH.size(); i++){
				if (PATH[i] == ':'){
					envVar += "/" + command;
					path p = envVar;

					if (exists(p) && is_regular_file(p)){
						auto perm = status(p).permissions();
						bool executable = ((perm & perms::owner_exec) != perms::none);

						if (executable) {
							cout << command << " is " << envVar << endl;
							found = true;
							break;
						}
					}

					envVar = "";
				}
				else envVar += PATH[i];
			}

			if (!found) cout << command << ": not found" << endl;
			
			continue;
		}
		
		cout << input << ": command not found" << endl;
	}

	return 0;
}
