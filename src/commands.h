#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

using namespace std;

void echo(vector<string> args);
void type(vector<string> args);
void pwd(vector<string> args);
void cd(vector<string> args);

extern unordered_map<string, function<void(vector<string>)>> commands;