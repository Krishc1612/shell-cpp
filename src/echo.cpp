#include "commands.h"

void echo(vector<string> args){
    int n = args.size();

    for (int i = 1; i < n; i++){
        cout << args[i] << " ";
    }
}