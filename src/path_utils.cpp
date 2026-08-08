#include "path_utils.h"
#include <filesystem>

using namespace std::filesystem;

pair<bool, string> hasValidExecutable(string& PATH, string& command){
    string envVar = "";

	for (int i = 0; i < PATH.size(); i++){
		if (PATH[i] == ':'){
			envVar += "/" + command;
			path p = envVar;

			if (exists(p) && is_regular_file(p)){
				auto perm = status(p).permissions();
				bool executable = ((perm & perms::owner_exec) != perms::none);

				if (executable) {
					return {true, envVar};
				}
			}

			envVar = "";
		}
		else envVar += PATH[i];
	}

	return {false, ""};
}