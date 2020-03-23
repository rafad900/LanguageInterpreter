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

Statements::~Statements() {
	std::vector<Statement *>::iterator ptr;
	for (ptr = _statements.begin(); ptr < _statements.end(); ptr++) 
		delete *ptr;
		// WOULD ALSO NEED TO ADD DESTRUCTOR TO EACH OF THE ExprNode CHILDREN CLASSES 
		// BUT THAT SEEMS UNECESSARY REALLY
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);
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

PrintStatement::PrintStatement() : _testlist{NULL} {}

PrintStatement::PrintStatement(std::vector<ExprNode *> testlist): _testlist{testlist} {}

std::vector<ExprNode *> PrintStatement::printString() {
	return _testlist;
}

void PrintStatement::evaluate(SymTab &symTab) {
	for (ExprNode *e: _testlist) {
		if (e->token().isName()) {
			TypeDescriptor *parent = symTab.getValueFor(e->token().getName());
			if (parent->type() == TypeDescriptor::INTEGER) 
				std::cout << dynamic_cast<IntegerTypeDescriptor *>(parent)->intValue();
			else if (parent->type() == TypeDescriptor::DOUBLE)
				std::cout << dynamic_cast<DoubleTypeDescriptor *>(parent)->doubleValue();
			else 
				std::cout << "\"" << dynamic_cast<StringTypeDescriptor *>(parent)->stringValue() << "\"";;
			std::cout << " ";
		} else {
			TypeDescriptor *parent = e->evaluate(symTab);
			if (parent->type() == TypeDescriptor::INTEGER) 
				std::cout << dynamic_cast<IntegerTypeDescriptor *>(parent)->intValue();
			else if (parent->type() == TypeDescriptor::DOUBLE)
				std::cout << dynamic_cast<DoubleTypeDescriptor *>(parent)->doubleValue();
			else 
				std::cout << "\"" << dynamic_cast<StringTypeDescriptor *>(parent)->stringValue() << "\"";
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

void PrintStatement::print() {
	std::cout << "print ";
	for (ExprNode *e :_testlist) {
		e->print();
		std::cout << " ";
	}
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
	while ( dynamic_cast<IntegerTypeDescriptor *>(_condition->evaluate(symTab))->intValue() ) {
 
		_stms->evaluate(symTab);
		_incdec->evaluate(symTab);
	}
}

void ForStatement::print() {
	std::cout << "for (";  _start->print(); std::cout << ";";  _condition->print(); std::cout << ";";  _incdec->print(); std::cout << ") {\n";
	_stms->print();
	std::cout << "}\n";
}

