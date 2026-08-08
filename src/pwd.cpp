#include "commands.h"

void pwd(vector<string> args){
    char path[1024];
    getcwd(path, sizeof(path));

    cout << path << endl;
}