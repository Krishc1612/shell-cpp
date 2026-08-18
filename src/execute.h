#pragma once

#include "commands.h"
#include "parse.h"

using namespace std;

bool execute(Task toExec);
int setupRedirect(Redirect& redirect);