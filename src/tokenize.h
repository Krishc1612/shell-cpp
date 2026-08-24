#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

enum TokenType {
    WORD,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_ERR,
    REDIRECT_OUT_APP,
    REDIRECT_ERR_APP,
    T_NULL
};

struct Token {
    TokenType type = WORD;
    string data = "";
};

vector<Token> tokenize(string& input);