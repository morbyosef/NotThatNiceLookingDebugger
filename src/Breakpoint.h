//
// Created by morbyosef on 1/9/19.
//

#ifndef DEBUGGER_BREAKPOINT_H
#define DEBUGGER_BREAKPOINT_H

#include <unistd.h>
#include <cstdint>
#include <sys/ptrace.h>
#include <iostream>

class Breakpoint {
public:
    Breakpoint(pid_t pid, std::intptr_t adress);

    void enable();
    void disable();

    bool is_enabled() const;
    std::intptr_t get_address() const;

private:
    std::intptr_t _break_address;
    pid_t _child_pid;
    bool _enabled;
    uint8_t _saved_data;
};


#endif //DEBUGGER_BREAKPOINT_H
