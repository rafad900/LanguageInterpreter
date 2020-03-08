//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"

void SymTab::setValueFor(std::string vName, TypeDescriptor *td) {
    // Define a variable by setting its initial value.
	if (td->type() == TypeDescriptor::BOOL) {
		BooleanTypeDescriptor *child = dynamic_cast<BooleanTypeDescriptor*>(td);
		std::cout << vName << " <- " << child->boolValue() << std::endl;
	} else if (td->type() == TypeDescriptor::INTEGER) {
		IntegerTypeDescriptor *child = dynamic_cast<IntegerTypeDescriptor *>(td);
		std::cout << vName << " = " << child->intValue() << std::endl;
	} else if (td->type() == TypeDescriptor::STRING) {
		StringTypeDescriptor *child = dynamic_cast<StringTypeDescriptor *>(td);
		std::cout << vName << " = " << child->stringValue() << std::endl;
	} else if (td->type() == TypeDescriptor::DOUBLE) {
		DoubleTypeDescriptor *child = dynamic_cast<DoubleTypeDescriptor *>(td);
		std::cout << vName << " = " << child->doubleValue() << std::endl;
	}
    symTab[vName] = td;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor* SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    if (symTab.find(vName)->second->type() == TypeDescriptor::BOOL) {
    	BooleanTypeDescriptor *child = dynamic_cast<BooleanTypeDescriptor*>(symTab.find(vName)->second);
	    std::cout << "SymTab::getValueFor: " << vName << " contains " << child->boolValue() << std::endl;
	    return child;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::INTEGER) {
		IntegerTypeDescriptor *child = dynamic_cast<IntegerTypeDescriptor*>(symTab.find(vName)->second);
		std::cout << "SymTab::getValueFor: " << vName << " contains " << child->intValue() << std::endl;
	    return child;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::STRING) {
		StringTypeDescriptor *child = dynamic_cast<StringTypeDescriptor*>(symTab.find(vName)->second);
		std::cout << "SymTab::getValueFor: " << vName << " contains " << child->stringValue() << std::endl;
	    return child;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::DOUBLE) {
		DoubleTypeDescriptor *child = dynamic_cast<DoubleTypeDescriptor*>(symTab.find(vName)->second);
	    std::cout << "SymTab::getValueFor: " << vName << " contains " << child->doubleValue() << std::endl;
	    return child;
	}	
	return nullptr;	
}

void SymTab::print() {
    for(auto [var, value] : symTab ) {
		if (value->type() == TypeDescriptor::BOOL) {
			BooleanTypeDescriptor *child = dynamic_cast<BooleanTypeDescriptor*>(value);
			std::cout << var << " = " << child->boolValue() << std::endl;
		} else if (value->type() == TypeDescriptor::INTEGER) {
			IntegerTypeDescriptor *child = dynamic_cast<IntegerTypeDescriptor *>(value);
			std::cout << var << " = " << child->intValue() << std::endl;
		} else if (value->type() == TypeDescriptor::STRING) {
		 	StringTypeDescriptor *child = dynamic_cast<StringTypeDescriptor *>(value);
			std::cout << var << " = " << child->stringValue() << std::endl;
		} else if (value->type() == TypeDescriptor::DOUBLE) {
			DoubleTypeDescriptor *child = dynamic_cast<DoubleTypeDescriptor *>(value);
			std::cout << var << " = " << child->doubleValue() << std::endl;
		}
        //std::cout << var << " = " << value << std::endl;
    }
}
