#include <vector>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#include "../ext-dependencies/linenoise/linenoise.h"

#include "linuxdebug.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Hello world" << std::endl;

    if(argc < 2) {
        std::cerr << "Program name not specified" << std::endl;
        return -1;
    }


    auto program = argv[1]; //second arg is command name

    /** LAUNCHING THE EXECUTABLE using fork/exec **/
    auto pid = fork(); //split program into two processes p1 p2
    if(pid == 0) {
        //In child process
        std::cout << "In child process---" << std::endl;

        /*
         * ptrace allows us to observe and control the execution of another process by reading * registers, reading memory, single stepping and more
         * PTRACE_TRACEME: indicates that this process should allow its parent to trace it
        */
        ptrace(PT_TRACE_ME, 0, nullptr, nullptr); // tracing child process pid 0

        execl(program, program, nullptr); //execute program, pass name as CLI arg, terminate list with nullptr
    }

    /** ADD DEBUGGER LOOP **/
    else if(pid >= 1) {
        //In parent process
        std::cout << "In parent process---" << std::endl;
        std::cout << "Started debugger process: " << pid << std::endl;

        linuxdebug::Debugger debug{program, pid};
        debug.run();
    }
    else {

    }

    return 1; 
}