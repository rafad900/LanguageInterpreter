//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"

void SymTab::setValueFor(std::string vName, TypeDescriptor *td) {
    // Define a variable by setting its initial value.
    if (isDefined(vName)) {
    	delete getValueFor(vName);
    }
	if (td->type() == TypeDescriptor::INTEGER) {
		std::cout << vName << " <- " << dynamic_cast<IntDescriptor*>(td)->intValue() << std::endl;
	} else if (td->type() == TypeDescriptor::STRING) {
		std::cout << vName << " <- " << "\"" << dynamic_cast<StrDescriptor*>(td)->stringValue() << "\"" << std::endl;
	} else if (td->type() == TypeDescriptor::DOUBLE) {
		std::cout << vName << " <- " << dynamic_cast<DblDescriptor*>(td)->doubleValue() << std::endl;
	}
    symTab[vName] = td;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

bool SymTab::isDefined(TypeDescriptor *value) {
	for (auto it = symTab.begin(); it != symTab.end(); ++it) 
		if (value == it->second)
			return true;
	return false;
}

TypeDescriptor* SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::INTEGER) {
		std::cout << "SymTab::getValueFor: " << vName << " contains " << dynamic_cast<IntDescriptor*>(symTab.find(vName)->second)->intValue() << std::endl;
	    return  dynamic_cast<IntDescriptor*>(symTab.find(vName)->second);
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::STRING) {
		std::cout << "SymTab::getValueFor: " << vName << " contains " << dynamic_cast<StrDescriptor*>(symTab.find(vName)->second)->stringValue() << std::endl;
	    return dynamic_cast<StrDescriptor*>(symTab.find(vName)->second);
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::DOUBLE) {
	  	std::cout << "SymTab::getValueFor: " << vName << " contains " << dynamic_cast<DblDescriptor*>(symTab.find(vName)->second)->doubleValue() << std::endl;
	    return dynamic_cast<DblDescriptor*>(symTab.find(vName)->second);
	}	
	return nullptr;	
}

void SymTab::print() {
    for(auto [var, value] : symTab ) {
		if (value->type() == TypeDescriptor::INTEGER) {
			std::cout << var << " = " << dynamic_cast<IntDescriptor*>(value)->intValue() << std::endl;
		} else if (value->type() == TypeDescriptor::STRING) {
			std::cout << var << " = " << "\"" << dynamic_cast<StrDescriptor*>(value)->stringValue() << "\"" << std::endl;
		} else if (value->type() == TypeDescriptor::DOUBLE) {
			std::cout << var << " = " << dynamic_cast<DblDescriptor*>(value)->doubleValue() << std::endl;
		}
    }
}

void SymTab::delete_descriptors() {
	std::map<std::string, TypeDescriptor *>::iterator it = symTab.begin();
	while (it != symTab.end()) {
		TypeDescriptor * value = it->second;
		delete value;
		it++;
	}
}
