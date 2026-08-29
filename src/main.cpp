#include <iostream>
#include <string>
#include <vector>
#include "tokenize.h"
#include "parse.h"
#include "execute.h"
#include <readline/readline.h>

using namespace std;

char** command_completion(const char*, int, int);
char* command_generator(const char*, int);

const char* completions[] = {
	"exit",
	"echo",
	NULL
};

int main() {
  // Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	rl_attempted_completion_function = command_completion;

	while (true){
		string input;
		char* buffer = readline("$ ");

		if (buffer){
			input = buffer;
			free(buffer);
		}

		vector<Token> tokens = tokenize(input);

		if (tokens.size()){
			Task toExec = parse(tokens);
			bool isExit = execute(toExec);

			if (isExit) break;
		}
	}

	return 0;
}

char** command_completion(const char* text, int start, int end){
	rl_attempted_completion_over = 1;
	return rl_completion_matches(text, command_generator);
}

char* command_generator(const char* text, int state){
	static int list_idx, len;
	const char* command;

	if(!state){
		list_idx = 0;
		len = strlen(text);
	}

	while ((command = completions[list_idx++])){
		if (strncmp(command, text, len) == 0){
			return strdup(command);
		}
	}

	return NULL;
}