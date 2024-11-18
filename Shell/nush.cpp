#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>


using namespace std;

int main() {
    string input;
    while (true) {
        // show the prompt
        cout << "nush$ ";

        // get input
        getline(cin, input);
        
        if (input.empty()) {
            continue;
        }

        // deal with exit
        if (input == "exit") {
            cout << "Exiting nush...\n";
            break;
        }

        // parse the input
        istringstream iss(input);
        string command;
        vector<string> commands;

        while (iss >> command) {
            commands.push_back(command);  // sotre the commands to vector
        }

        // create child process
        pid_t pid = fork();
        if (pid < 0) { // fork failed; exit
            cerr << "nush error: Fork failed" << endl;
            exit(1);
        }else if (pid == 0) {  // child (new process)
            vector<char*> args;
            for (const auto& cmd : commands) {
                args.push_back(const_cast<char*>(cmd.c_str()));  // convert to C string
            }
            args.push_back(nullptr);  // execvp 参数最后一个必须为 NULL

            // Try to execute the command using execvp
            if (execvp(args[0], args.data()) == -1) {
                // If execvp fails, handle the error based on errno
                if (errno == ENOENT) {
                    cerr << "nush error: Command not found: " << args[0] << endl;
                    exit(127);  // Command not found (127 is a common error code for this)
                } else {
                    cerr << "nush error: Failed to execute: " << strerror(errno) << endl;
                    exit(126);  // Failed to execute, but command was valid (126 is a common code for this)
                }
            }


        } else {
            // parent goes down this path (main)
            //need to wait children
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                cout << "nush status: " << WEXITSTATUS(status) << endl;
                if (exit_code == 0){
                    cout << "nush: Command executed successfully." << endl;
                }
                else if (exit_code == 127) {
                    cout << "nush: Error - Command not found." << endl;
                } else if (exit_code == 126) {
                    cout << "nush: Error - Command execution failed." << endl;
                }
                else if (exit_code != 0) {
                    cout << "nush: Command returned non-zero exit code: " << exit_code << endl;
                }
            } else {
                cout << "nush error: Child process did not terminate normally" << endl;
            }
        }
    }
    return 0;
}
