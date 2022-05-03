#ifndef LINUX_DEBUGGER_HPP
#define LINUX_DEBUGGER_HPP

#include <utility>
#include <string>
// #include <linux/types.h>

namespace linuxdebug {

    class Debugger {
         public:
         //Constructor
         Debugger(std::string program_name, pid_t pid) : m_program_name{std::move(program_name)}, m_pid{pid} {};
         
         void run();

    private:
        std::string m_program_name;
        pid_t m_pid;
    };
}

#endif