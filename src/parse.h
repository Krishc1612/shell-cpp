#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "tokenize.h"

using namespace std;

struct Redirect{
    TokenType type = T_NULL;
    string target = "";
};

struct Task{
    string command = "";
    vector<string> args = {};
    Redirect redirect;
};

Task parse(vector<Token> tokens);