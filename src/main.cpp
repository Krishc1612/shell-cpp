#include <iostream>
#include <string>
#include <vector>
#include "tokenize.h"
#include "parse.h"
#include "execute.h"

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	while (true){
		cout << "$ ";

		string input;
		getline(cin, input);

		vector<Token> tokens = tokenize(input);

		if (tokens.size()){
			Task toExec = parse(tokens);
			bool isExit = execute(toExec);

			if (isExit) break;
		}
	}

	return 0;
}