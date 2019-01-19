#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <string>
#include "debugger.h"

#define CHILD_PROCESS_ID (0)

int main(int argc, char * argv[]) {

    if(argc < 2) {
            std::cout << "Please enter program name" << std::endl;
            return -1;
    }

    char* programName = argv[1];
    pid_t pid = fork();

    if(pid == CHILD_PROCESS_ID){
        ptrace(PTRACE_TRACEME, CHILD_PROCESS_ID, nullptr, nullptr);
        execl(programName, programName, nullptr);
    }
    
    else if (pid >= 1){
        debugger dbg(programName, pid);
        dbg.run(); // runs the debugger
    }

    return 0;
    
}
