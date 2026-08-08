#include "commands.h"
#include "path_utils.h"

void type(vector<string> args){
    string command = args[1];

    if (commands.find(command) != commands.end()){
        cout << command << " is a shell builtin" << endl;
        return;
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