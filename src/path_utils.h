#pragma once

#include <iostream>
#include <string>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

pair<bool, string> hasValidExecutable(string& PATH, string& command);