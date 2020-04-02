//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include <stdio.h>
#include <string>
#include "Range.hpp"
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

ForStatement::ForStatement() : _testlist{ NULL }, _stms{ nullptr }, _id{ "\0" } {}

ForStatement::ForStatement(std::string id, std::vector<ExprNode*> testlist, Statements *forStms):
	_testlist{ testlist }, _stms{ forStms }, _id{ id } 
{
	if (_testlist.size() > 3 || _testlist.size() < 1) {
		std::cout << "Only 3 parameters can be used in range(). Instead " << _testlist.size() << " were passed.\n";
		exit(1);
	}
}

Statements *&ForStatement::stms() {
	return _stms;
}
void ForStatement::evaluate(SymTab& symTab) {
	Range* rg;

	for (int i = 0; i < _testlist.size(); i++) {
		TypeDescriptor* parent = _testlist[i]->evaluate(symTab);
		if (parent->type() != TypeDescriptor::INTEGER) {
			std::cout << "parameters for range have to be integers\n";
			exit(1);
		}
		else {
			parameters.push_back(dynamic_cast<IntegerTypeDescriptor*>(parent)->intValue());
		}
	}

	if (parameters.size() == 1)
		rg = new Range(parameters[0]);
	else if (parameters.size() == 2)
		rg = new Range(parameters[0], parameters[1]);
	else
		rg = new Range(parameters[0], parameters[1], parameters[2]);
	
	while (rg->condition()) {
		symTab.setValueFor(_id, new IntegerTypeDescriptor(rg->current()));
		_stms->evaluate(symTab);
		rg->next();
	}
}

void ForStatement::print() {

	std::cout << "for " << _id << " in range(";	
	for (int i = 0; i < _testlist.size() - 1; i++) {
		_testlist[i]->print();
		std::cout << ", ";
	}
	_testlist[_testlist.size() - 1]->print();
	std::cout << ") { \n";
	_stms->print();
	std::cout << "}\n";
}

IfStatement::IfStatement() : testSuites{ NULL } {}

void IfStatement::evaluate(SymTab &symTab) {
	int a;
}

void IfStatement::print() {
	std::cout << "This is for if statement\n";
}

void IfStatement::insertSuite(ExprNode* test, Statements* stms) {
	testSuites.push_back(std::make_pair(test, stms));
}