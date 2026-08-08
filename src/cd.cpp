#include "commands.h"

void cd(vector<string> args){
    path p = args[1];

    if (exists(p) && is_directory(p)){
        const char* changeTo = args[1].c_str();

        chdir(changeTo);
    }
    else {
        cout << "cd: " << args[1] << ": No such file or directory" << endl;
    }
}