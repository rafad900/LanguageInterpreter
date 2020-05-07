//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
#include <exception>

void SymTab::setValueFor(std::string vName, TypeDescriptor* td) {
	// Define a variable by setting its initial value.
	if (isDefined(vName)) {
		delete getValueFor(vName);
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
	    return  symTab.find(vName)->second;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::STRING) {
	    return symTab.find(vName)->second;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::DOUBLE) {
	    return symTab.find(vName)->second;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::ARRAY) {
		return symTab.find(vName)->second;
	} else if (symTab.find(vName)->second->type() == TypeDescriptor::FUNC) {
		return symTab.find(vName)->second;
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
		else if (value->type() == TypeDescriptor::ARRAY) {
			dynamic_cast<ArrDescriptor*>(value)->print(); std::cout << std::endl;
		}
		else if (value->type() == TypeDescriptor::FUNC) {
			std::cout << var << " contains a function\n";
		}
    }
}


void SymTab::operator = (const SymTab& OtherTable) {  // OVERLOADED OPERATOR USED FOR WHEN CREATING SYMBOL TABLES OF DIFFERENT SCOPES
	
	for (auto const& x : OtherTable.symTab) {
		if (x.second->type() == TypeDescriptor::INTEGER) {
			int temp = dynamic_cast<IntDescriptor*>(x.second)->intValue();
			this->setValueFor(x.first, new IntDescriptor(temp));
		}
		else if (x.second->type() == TypeDescriptor::STRING) {
			std::string temp = dynamic_cast<StrDescriptor*>(x.second)->stringValue();
			this->setValueFor(x.first, new StrDescriptor(temp));
		}
		else if (x.second->type() == TypeDescriptor::DOUBLE) {
			double temp = dynamic_cast<DblDescriptor*>(x.second)->doubleValue();
			this->setValueFor(x.first, new DblDescriptor(temp));
		}
		else if (x.second->type() == TypeDescriptor::ARRAY) {
			std::vector<TypeDescriptor*> temp;
			dynamic_cast<ArrDescriptor*>(x.second)->testlistCopy(temp);
			this->setValueFor(x.first, new ArrDescriptor(temp));
		}
		else if (x.second->type() == TypeDescriptor::FUNC) {
			this->setValueFor(x.first, x.second);
		}
	}

}

void SymTab::delete_descriptors() {
	for (auto it = symTab.begin(); it != symTab.end(); ++it) {
		if (it->second->type)
			delete it->second;
		it->second = NULL;
	}
}
