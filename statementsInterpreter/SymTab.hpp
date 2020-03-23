//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include "TypeDescriptors.hpp"

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class SymTab {
public:
    void setValueFor(std::string vName, TypeDescriptor *td);
    bool isDefined(std::string vName);
    bool isDefined(TypeDescriptor *value);
    TypeDescriptor* getValueFor(std::string vName);
    void print();
    ~SymTab();

private:
    std::map<std::string, TypeDescriptor *> symTab;
};

#endif //EXPRINTER_SYMTAB_HPP
