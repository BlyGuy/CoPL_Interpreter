#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <vector>
#include <string>

struct SymTableEntry
{
    std::string varName = "";
    bool bound = false;
    size_t bound_id = 0;
};

#endif SYMTABLE_H