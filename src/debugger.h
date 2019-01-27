//
// Created by morbyosef on 1/9/19.
//

#ifndef UNTITLED_DEBUGGER_H
#define UNTITLED_DEBUGGER_H

#include <string>
#include <sys/wait.h>
#include <iomanip>
#include <vector>
#include <iostream>
#include <sys/ptrace.h>
#include <unordered_map>
#include "Helper.h"
#include "Breakpoint.h"
#include "Registers.h"
#include "linenoise.h"
#include "libelfin/dwarf/dwarf++.hh"
#include "libelfin/elf/elf++.hh"

class debugger {
public:
    debugger(std::string, pid_t);
    void run();

private:
    void handle_command(const std::string &command);
    void continue_child();
    void create_breakpoint(std::intptr_t);
    void dump_registers();
    uint64_t read_memory(uint64_t address);
    void write_memory(uint64_t address, uint64_t data);
    uint64_t get_pc();
    void set_pc(uint64_t pc);
    void step_over_breakpoint();
    void wait_for_signal();
    void print_help();
    dwarf::die get_function_from_pc(uint64_t pc);
    dwarf::line_table::iterator get_line_entry_from_pc(uint64_t);
    void print_soruce(std::string &file_name, unsigned line, unsigned lines_context);

    pid_t _process_pid;
    std::string _process_name;
    dwarf::dwarf _process_dwarf;
    elf::elf _process_elf;
    std::unordered_map<std::intptr_t ,Breakpoint> _process_breakpoints;
};


#endif //UNTITLED_DEBUGGER_H
