#include <vector>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#include "../ext-dependencies/linenoise/linenoise.h"

#include "linuxdebug.hpp"

/* Runs the debugger process */
void linuxdebug::Debugger::run() {
    int waitStatus;
    auto options = 0;

    /* When the traced/child process is launched, it will be sent a SIGTRAP signal, which is a trace or breakpoint trap. We can wait until this signal is sent using the waitpid function. */
    waitpid(linuxdebug::Debugger::m_pid, &waitStatus, options); //wait until child process pid 0 has finished launching

    // Listen for user input from linenoise until we hit EOF / Ctrl + D.
    char *line = nullptr;
    while((line = linenoise("linuxdebugger> ")) != nullptr) {
        handle_command(line); //TODO: handle input
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

/** Executes the debugger on the program given as an input. Prints error when ptrace errors out. **/
void execute_debugger_program(const std::string& program_name) {
    /*
     * ptrace allows to observe and control the execution of another process by reading 
     *registers, reading memory, single stepping and more 
     * PTRACE_TRACEME: indicates that this process should allow its parent to trace it
    */
    long ptrace_val = ptrace(PTRACE_TRACEME, 0, nullptr, nullptr); // tracing child process pid 0
    if(ptrace_val < 0) {
        std::cerr << "Error in ptrace" << std::endl;
    }

    execl(program_name.c_str(), program_name.c_str(), nullptr); //execute program, pass name as CLI arg, terminate list with nullptr
}



int main(int argc, char* argv[]) {
    std::cout << "Hello world" << std::endl;

    if(argc < 2) {
        std::cerr << "Program name not specified" << std::endl;
        return -1;
    }


    auto program = argv[1]; //second arg is command name

    /** 1. LAUNCHING THE EXECUTABLE using fork/exec **/
    auto pid = fork(); //split program into two processes p1 p2
    if(pid == 0) {
        //In child process
        std::cout << "In child process---" << std::endl;
        execute_debugger_program(program);
    }

    /** 2. ADD DEBUGGER LOOP **/
    else if(pid >= 1) {
        //In parent process
        std::cout << "In parent process---" << std::endl;
        std::cout << "Started debugger process: " << pid << std::endl;

        linuxdebug::Debugger debug{program, pid};
        debug.run();
    }

    return 1; 
}