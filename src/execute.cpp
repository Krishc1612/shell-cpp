#include "execute.h"
#include "path_utils.h"
#include <sys/wait.h>
#include <fcntl.h>

unordered_map<string, function<void(vector<string>)>> commands = {
	{"exit", shellExit},
	{"echo", echo},
	{"type", type},
	{"pwd", pwd},
	{"cd", cd}
};

unordered_map<TokenType, int> stdFd = {
    {REDIRECT_OUT, STDOUT_FILENO},
    {REDIRECT_ERR, STDERR_FILENO},
    {REDIRECT_IN, STDIN_FILENO},
    {REDIRECT_OUT_APP, STDOUT_FILENO},
    {REDIRECT_ERR_APP, STDERR_FILENO}
};

int setupRedirect(Redirect& redirect){
    int savedFd = -1;
    int flags;

    if (
        redirect.type == REDIRECT_OUT ||
        redirect.type == REDIRECT_ERR
    ) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    }
    else if (redirect.type == REDIRECT_IN) {
        flags = O_RDONLY;
    }
    else if (
        redirect.type == REDIRECT_OUT_APP ||
        redirect.type == REDIRECT_ERR_APP
    ) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else {
        return -1;
    } // setting required flags according to the redirect

    int fileFd = open(redirect.target.c_str(), flags, 0644); // opening the target file to get its fd

    if (fileFd == -1){
        throw runtime_error("Unable to open redirection target"); // opening failed
    }

    int targetFd;
    // mapping TokenType to the actual std_fileno (default settings)
    if (stdFd.find(redirect.type) != stdFd.end()){
        targetFd = stdFd[redirect.type];
    }
    else return -1;

    savedFd = dup(targetFd); // save the current place or fd where the result is going
    if (savedFd == -1){ // saving failed
        close(fileFd);
        throw runtime_error("dup failed");
    }

    if (dup2(fileFd, targetFd) == -1){ // make the targetFd refer to the same place which fileFd refers to (target file)
        close(savedFd); // above process failed
        close(fileFd);

        throw runtime_error("dup2 failed");
    }

    close(fileFd); // no need of this now, as targetFd is pointing to the file - we can access it for any other operation.

    return savedFd; // finally return the savedFd
}

bool execute(Task toExec){
    if (toExec.command == "exit") return true;

    int savedFd = -1; 
    if (toExec.redirect.type != T_NULL){
        savedFd = setupRedirect(toExec.redirect);
    }

    if (commands.find(toExec.command) != commands.end()){
        commands[toExec.command](toExec.args);
    }
    else {
        string PATH = getenv("PATH");
        PATH += ":";

        auto [isValid, pathTo] = hasValidExecutable(PATH, toExec.command);

        if (isValid){
            if (fork() == 0){
                const char* p = pathTo.data();
                vector<char*> execArgs;

                for (auto& arg: toExec.args){
                    execArgs.push_back(arg.data());
                }
                execArgs.push_back(nullptr);

                execv(p, execArgs.data());
                _exit(1);
            }
            else {
                wait(nullptr);
            }
        }
        else {
            cerr << toExec.command << ": not found" << endl;
        }
    }

    if (savedFd != -1){ // means that redirection happened, so we need to restore it in the shell process
        int targetFd = stdFd[toExec.redirect.type];

        dup2(savedFd, targetFd);
        close(savedFd);
    }

    return false;
}