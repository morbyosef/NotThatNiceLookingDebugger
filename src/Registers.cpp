//
// Created by morbyosef on 1/10/19.
//

#include "Registers.h"

uint64_t get_register_value(pid_t pid, reg r){
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    auto it = std::find_if(begin(register_descriptors), end(register_descriptors), [r](const register_descriptor rd){
        return rd.r == r;
    });

    return *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(register_descriptors)));
}

void set_register_value(pid_t pid, reg r, uint64_t data){
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    auto it = std::find_if(begin(register_descriptors), end(register_descriptors), [r](const register_descriptor rd){
        return rd.r == r;
    });
    *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(register_descriptors))) = data;

    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

uint64_t  get_register_value_from_dwarf(pid_t pid, unsigned regnum){
    auto it = std::find_if(begin(register_descriptors), end(register_descriptors), [regnum](const register_descriptor rd)
    { return rd.dwarf == regnum; });

    if(it == end(register_descriptors)){
        throw std::out_of_range("Unknown dwarf register");
    }

    return get_register_value(pid, it->r);
}


std::string get_register_name(reg r) {
    auto it = std::find_if(begin(register_descriptors), end(register_descriptors),
                           [r](const register_descriptor rd) { return rd.r == r; });
    return it->name;
}

reg get_register_from_name(const std::string& name) {
    auto it = std::find_if(begin(register_descriptors), end(register_descriptors),
                           [name](const register_descriptor rd) { return rd.name == name; });
    return it->r;
}



