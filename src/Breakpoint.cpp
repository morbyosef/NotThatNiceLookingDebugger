//
// Created by morbyosef on 1/9/19.
//

#include "Breakpoint.h"

Breakpoint::Breakpoint(pid_t pid, std::intptr_t address) : _child_pid(pid), _break_address(address) {}

void Breakpoint::enable() {
    auto data = ptrace(PTRACE_PEEKDATA, this->_child_pid, this->_break_address, nullptr); // take data from the code
    this->_saved_data = static_cast<uint8_t >(data & 0xff);
    uint64_t int3 = 0xcc;
    uint64_t data_int3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, this->_child_pid, this->_break_address, data_int3); // replace the code and put int 3 at the beginning

    this->_enabled = true;
}

void Breakpoint::disable() {
    auto data = ptrace(PTRACE_PEEKDATA, this->_child_pid, this->_break_address, nullptr); // take the breakpoint from the code
    auto restored = ((data & ~0xff) | data);

    ptrace(PTRACE_POKEDATA, this->_child_pid, this->_break_address, restored); // restore the code to the previous state
    this->_enabled = false;
}

bool Breakpoint::is_enabled() const {
    return this->_enabled;
}

std::intptr_t Breakpoint::get_address() const {
    return this->_break_address;
}