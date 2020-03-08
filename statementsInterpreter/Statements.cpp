//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include <stdio.h>
#include <string>
// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements) {
        s->print();
        std::cout << std::endl;
    }
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
	// Calculate the thing afte the equal sign
    TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);
    /* 
     * MIGHT NOT NEED THIS SINCE THE RETURN VALUE IS ALREADY A TYPE DESCRIPTOR WITH ITS OWN CHILD SAVED INTO IT
    TypeDescriptor *td;
    if (typeid(rhs) == typeid(int)) {
    	IntegerTypeDescriptor *temp = new IntegerTypeDescriptor(lhsVariable(), rhs);
    	td = temp;
    } else if (typeid(rhs) == typeid(double)) {
    	DoubleTypeDescriptor *temp = new DoubleTypeDescriptor(lhsVariable(), rhs);
    	td = temp;
    } else if (typeid(rhs) == typeid(bool)) {
    	BooleanTypeDescriptor *temp = new BooleanTypeDescriptor(lhsVariable(), rhs);
    	td = temp;
    } else {
    	rhs = (std::string) rhs;
    	StringTypeDescriptor *temp = new StringTypeDescriptor(lhsVariable(), rhs);
    	td = temp;
    }
    */
    // Then set the value of the variable to the rhs
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
}

PrintStatement::PrintStatement() : _printString{nullptr}, _var{0}, _name{"\0"} {}

PrintStatement::PrintStatement(ExprNode *s, std::string name):
		_printString{s}, _name{name} {}

ExprNode *&PrintStatement::printString() {
	return _printString;
}

std::string &PrintStatement::getVarName() {
	return _name;
}

void PrintStatement::evaluate(SymTab &symTab) {
	_var = symTab.getValueFor(_name);
	std::cout << _var << std::endl;
}

void PrintStatement::print(bool indent) {
	std::cout << "print " << _printString->token().getName();
}

ForStatement::ForStatement() : _start{nullptr}, _incdec{nullptr}, _condition{nullptr}, _stms{nullptr} {}

ForStatement::ForStatement(AssignmentStatement *start, AssignmentStatement *incdec, ExprNode * condition, Statements *forStms):
 _start{start}, _incdec{incdec}, _condition{condition}, _stms{forStms} {}

AssignmentStatement *&ForStatement::start() {
	return _start;
}

AssignmentStatement *&ForStatement::incdec() {
	return _incdec;
}

ExprNode *&ForStatement::condition() {
	return _condition;
}

Statements *&ForStatement::stms() {
	return _stms;
}

void ForStatement::evaluate(SymTab &symTab) {
	_start->evaluate(symTab);
	while (_condition->evaluate(symTab)) { 
		_stms->evaluate(symTab);
		_incdec->evaluate(symTab);
	}
}

void ForStatement::print() {
	std::cout << "for (";  _start->print(); std::cout << ";";  _condition->print(); std::cout << ";";  _incdec->print(); std::cout << ") {\n";
	_stms->print();
	std::cout << "}\n";
}

