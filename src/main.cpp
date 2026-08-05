#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  unordered_set<string> builtins = {
    "exit",
    "echo",
    "type"
  };

  while (true){
    cout << "$ ";

    string input;
    getline(cin, input);

    if (input == "exit") break;

    if (input.substr(0, 5) == "echo "){
      cout << input.substr(5, input.size()) << endl;
      continue;
    }

    if (input.substr(0, 5) == "type "){
      string command = input.substr(5);

      if (builtins.find(command) != builtins.end()){
        cout << command << " is a shell builtin";
      }
      else {
        cout << command << ": not found";
      }
    }
    
    cout << input << ": command not found" << endl;
  }

  return 0;
}
