//
// Created by morbyosef on 1/9/19.
//

#include <fcntl.h>
#include "debugger.h"

debugger::debugger(std::string process_name, pid_t pid) : _process_name(process_name), _process_pid(pid) {
    //auto filedwarf = open(process_name.c_str(), O_RDONLY);

    //this->_process_elf = elf::elf(elf::create_mmap_loader(filedwarf));
    //this->_process_dwarf = dwarf::dwarf(dwarf::elf::create_loader(this->_process_elf));
}

void debugger::run() {
    int settings = 0;
    int wait_status;
    char * command;

    waitpid(this->_process_pid, &wait_status, settings);

    while((command = linenoise("MorDebugger> ")) != nullptr){
        this->handle_command(command);
        linenoiseHistoryAdd(command);
        linenoiseFree(command);
    }
}

void debugger::print_help() {
    std::cout << "Commands Usage:" << std::endl << std::endl;
    std::cout << "  help: print this screen" << std::endl;
    std::cout << "  continue: continue execution" << std::endl;
    std::cout << "  break <0xaddress>: craete a breakpoint at address" << std::endl;
    std::cout << "  register:" << std::endl;
    std::cout << "      dump: print all registers" << std::endl;
    std::cout << "      read <register_name>: print the register value" << std::endl;
    std::cout << "      write <register_name> <0xvalue>: write the value to the register" << std::endl;
    std::cout << "  memory:" << std::endl;
    std::cout << "      read <0xaddress>: read memory from address" << std::endl;
    std::cout << "      write <0xaddress> <0xvalue>: write the value to the address memory" << std::endl << std::endl;
}
void debugger::handle_command(const std::string &line) { // split the line by the delimiter
    std::vector<std::string> arguments = Helper::splitString(line, ' ');
    std::string command = arguments[0];

    if(Helper::check_prefix(command, "continue")){
        this->continue_child();
    }
    else if(Helper::check_prefix(command, "break")){
        std::string address(arguments[1], 2);
        this->create_breakpoint(std::stol(address ,0 ,16));
    }
    else if(Helper::check_prefix(command, "register")){
        if(Helper::check_prefix(arguments[1], "dump")){
            this->dump_registers();
        }
        else if(Helper::check_prefix(arguments[1], "read")){
            std::cout << get_register_value(this->_process_pid, get_register_from_name(arguments[2])) << std::endl;
        }
        else if(Helper::check_prefix(arguments[1], "write")){
            std::string data(arguments[3], 2);
            set_register_value(this->_process_pid, get_register_from_name(arguments[2]), std::stol(data, 0 ,16));
        }
    }
    else if(Helper::check_prefix(command, "memory")){
        std::string address(arguments[2], 2);
        if(Helper::check_prefix(arguments[1], "read")){
            std::cout << std:: hex << this->read_memory(std::stol(address, 0, 16)) << std::endl;
        }
        else if(Helper::check_prefix(arguments[1], "write")) {
            std::string data(arguments[3], 2);
            this->write_memory(std::stol(address, 0, 16), std::stol(data, 0, 16));
        }
    }
    else if(Helper::check_prefix(command, "help")){
        this->print_help();
    }
    else{
        std::cout << "Unknown command " << std::endl;
    }

}

void debugger::continue_child() {
    this->step_over_breakpoint();
    ptrace(PTRACE_CONT, this->_process_pid, nullptr, nullptr); // continue the execution
    this->wait_for_signal();
}

void debugger::create_breakpoint(std::intptr_t address) {
    std::cout << "Breakpoint created at 0x" << std::hex << address << std::endl;

    Breakpoint bp(this->_process_pid, address);// create a new breakpoint
    bp.enable();

    this->_process_breakpoints.insert(std::pair<std::intptr_t, Breakpoint>(address, bp)); // insert the breakpoint to the map
}

void debugger::dump_registers() {
    for(const auto & reg : register_descriptors){
        std::cout << reg.name <<  " 0x" << std::setfill('0') << std::setw(16) << std::hex << get_register_value(this->_process_pid, reg.r) << std::endl;
    }
}

uint64_t debugger::read_memory(uint64_t address) {
    return ptrace(PTRACE_PEEKDATA, this->_process_pid, address, nullptr);
}

void debugger::write_memory(uint64_t address, uint64_t data) {
    ptrace(PTRACE_POKEDATA, this->_process_pid, address, data);
}

uint64_t debugger::get_pc(){
    return get_register_value(this->_process_pid, reg::rip);
}

void debugger::set_pc(uint64_t pc) {
    set_register_value(this->_process_pid, reg::rip, pc);
}

void debugger::step_over_breakpoint() {
    auto bp_position = this->get_pc() - 1; // optional breakpoint address

    if(this->_process_breakpoints.count(bp_position)){
        auto &bp = this->_process_breakpoints.at(bp_position);

        if(bp.is_enabled()){ // if bp is enabled disable step over and enable again

            set_pc(bp_position);

            bp.disable();
            ptrace(PTRACE_SINGLESTEP, this->_process_pid, nullptr, nullptr);
            this->wait_for_signal();
            bp.enable();
        }
    }
}

void debugger::wait_for_signal() {
    int wait_status;
    auto options = 0;
    waitpid(this->_process_pid , &wait_status, options);
}

/*
dwarf::die debugger::get_function_from_pc(uint64_t pc) {
    for(auto &cu : this->_process_dwarf.compilation_units()){ // iterate threw compilation units until finding one with the pc in it
        if(dwarf::die_pc_range(cu.root()).contains(pc)){
            for(const auto& die : cu.root()){
                if(die.tag ==  dwarf::DW_TAG::subprogram){
                    if(dwarf::die_pc_range(die).contains(pc)){
                        return die;
                    }
                }
            }
        }
    }

    throw std::out_of_range("Function not found");
}

dwarf::line_table::iterator debugger::get_line_entry_from_pc(uint64_t pc) {
    for(auto &cu : this->_process_dwarf.compilation_units()){
        if(dwarf::die_pc_range(cu.root()).contains(pc)){
            auto &line_table = cu.get_line_table();
            auto address = line_table.find_address(pc);
            if(address == line_table.end()){
                throw std::out_of_range("Cant find line entry");
            }
            else{
                 return address;
            }
        }
    }

    throw std::out_of_range("Cant find line entry");
}

void debugger::print_soruce(std::string &file_name, unsigned line, unsigned lines_context) {
    auto start = line <= lines_context ? 1: line - lines_context;
    auto end = line +lines_context  + (line < lines_context ? lines_context -line : 0) + 1;
    char c;
}
 */