//
// Created by morbyosef on 1/10/19.
//

#ifndef DEBUGGER_REGISTERS_H
#define DEBUGGER_REGISTERS_H


#include <cstddef>
#include <string>
#include <sys/user.h>
#include <array>
#include <sys/ptrace.h>
#include <algorithm>
#include <vector>

enum class reg{
    rax, rbx, rcx, rdx,
    rdi, rsi, rbp, rsp,
    r8,  r9,  r10, r11,
    r12, r13, r14, r15,
    rip, rflags,    cs,
    orig_rax, fs_base,
    gs_base,
    fs, gs, ss, ds, es
};

constexpr std::size_t registers_count = 27;

struct register_descriptor{
    reg r;
    int dwarf;
    std::string name;
};

const std::array<register_descriptor, registers_count> register_descriptors {{
  { reg::r15, 15, "r15" },
  { reg::r14, 14, "r14" },
  { reg::r13, 13, "r13" },
  { reg::r12, 12, "r12" },
  { reg::rbp, 6, "rbp" },
  { reg::rbx, 3, "rbx" },
  { reg::r11, 11, "r11" },
  { reg::r10, 10, "r10" },
  { reg::r9, 9, "r9" },
  { reg::r8, 8, "r8" },
  { reg::rax, 0, "rax" },
  { reg::rcx, 2, "rcx" },
  { reg::rdx, 1, "rdx" },
  { reg::rsi, 4, "rsi" },
  { reg::rdi, 5, "rdi" },
  { reg::orig_rax, -1, "orig_rax" },
  { reg::rip, -1, "rip" },
  { reg::cs, 51, "cs" },
  { reg::rflags, 49, "eflags" },
  { reg::rsp, 7, "rsp" },
  { reg::ss, 52, "ss" },
  { reg::fs_base, 58, "fs_base" },
  { reg::gs_base, 59, "gs_base" },
  { reg::ds, 53, "ds" },
  { reg::es, 50, "es" },
  { reg::fs, 54, "fs" },
  { reg::gs, 55, "gs" },
}};

uint64_t get_register_value(pid_t pid, reg r);
void set_register_value(pid_t pid, reg r, uint64_t data);
uint64_t  get_register_value_from_dwarf(pid_t pid, unsigned regnum);
std::string get_register_name(reg r);
reg get_register_from_name(const std::string& name);

#endif //DEBUGGER_REGISTERS_H
