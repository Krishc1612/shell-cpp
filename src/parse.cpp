#include "parse.h"
#include <error.h>

Task parse(vector<Token> tokens){
    int n = tokens.size();

    Task toExec;
    string errorMsg = "";

    if (tokens[0].type != WORD){
        errorMsg += "Syntax error.";
        throw runtime_error(errorMsg);
    } 

    toExec.command = tokens[0].data;
    bool isTarget = false;

    for (Token& token : tokens){
        if (isTarget){
            toExec.redirect.target = token.data;
            continue;
        }

        if (token.type == WORD){
            toExec.args.push_back(token.data);
        }
        else {
            isTarget = true;
            toExec.redirect.type = token.type;
        }
    }

    return toExec;
}