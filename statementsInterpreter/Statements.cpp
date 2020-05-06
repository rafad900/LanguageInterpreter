//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include <stdio.h>
#include <string>
#include "Range.hpp"
// Statement
Statement::Statement() {}

Statement::~Statement() {}

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
	for (auto s : _statements) {
		delete s;
	}
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{ "" }, _index{ -1 }, _rhsExpression{ nullptr }, _array{ 0 } {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, int index, ExprNode* rhsExpr) :
	_lhsVariable{ lhsVar }, _rhsExpression{ rhsExpr }, _index{ index }, _array{ 0 } {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, std::vector<ExprNode *> rhsExpr): 
	_lhsVariable{ lhsVar }, _rhsExpression{ nullptr }, _index{ -1 }, _array{ rhsExpr } {}

void AssignmentStatement::evaluate(SymTab &symTab) {

	if (_index != -1) {
		TypeDescriptor* array = symTab.getValueFor(lhsVariable());
		if (array->type() != TypeDescriptor::ARRAY) {
			std::cout << "\nThe variable " << lhsVariable() << " does not hold an array.";
			exit(0);
		}
		ArrDescriptor* arrayDes = dynamic_cast<ArrDescriptor*>(array);
		arrayDes->testlist()[_index] = rhsExpression()->evaluate(symTab);
	}
	else if (rhsExpression() == nullptr) {
		std::vector<TypeDescriptor*> testlist;
		TypeDescriptor::types type;
		type = _array[0]->evaluate(symTab)->type();
		for (int i = 0; i < (int)_array.size(); i++) {
			TypeDescriptor* value = _array[i]->evaluate(symTab);
			if (type != value->type()) {
				std::cout << "The types of a list must be the same";
				exit(1);
			}
			testlist.push_back(value);
		}
		symTab.setValueFor(lhsVariable(), new ArrDescriptor(testlist));
	}
	else {
		TypeDescriptor* rhs = rhsExpression()->evaluate(symTab);
		if (rhs->type() == TypeDescriptor::ARRAY) {
			std::vector<TypeDescriptor*> copy;
			dynamic_cast<ArrDescriptor*>(rhs)->testlistCopy(copy);
			rhs = new ArrDescriptor(copy);
			symTab.setValueFor(lhsVariable(), rhs);
		} 
		else
			symTab.setValueFor(lhsVariable(), rhs);
	}
}

int& AssignmentStatement::index() {
	return _index;
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

std::vector<ExprNode*> AssignmentStatement::array() {
	return _array;
}

void AssignmentStatement::print() {
	if (_index != -1)
		std::cout << _lhsVariable << "[" << _index << "] = ";
	else
		std::cout << _lhsVariable << " = ";

	if (_rhsExpression == nullptr) {
		std::cout << "[";				// THIS CODE BELONDS INSIDE THE ARRAY TYPE DESCRIPTOR
		int size = _array.size()-1;
		for (int i = 0; i < size; i++) {
			_array[i]->print();
			std::cout << ",";
		}
		if (_array.size() > 0)
			_array[_array.size() - 1]->print();
		std::cout << "]";
	}
	else 
		_rhsExpression->print();
}

AssignmentStatement::~AssignmentStatement() {
	delete _rhsExpression;
}

// Print Statement 

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
				std::cout << dynamic_cast<IntDescriptor*>(parent)->intValue();
			else if (parent->type() == TypeDescriptor::DOUBLE)
				std::cout << dynamic_cast<DblDescriptor*>(parent)->doubleValue();
			else if (parent->type() == TypeDescriptor::STRING)
				std::cout << "\"" << dynamic_cast<StrDescriptor*>(parent)->stringValue() << "\"";
			else if (parent->type() == TypeDescriptor::ARRAY)
				dynamic_cast<ArrDescriptor*>(parent)->print(); std::cout << std::endl;
			std::cout << " ";
			if (!symTab.isDefined(parent))
				delete parent;
		} else {
			TypeDescriptor *parent = e->evaluate(symTab);
			if (parent->type() == TypeDescriptor::INTEGER) 
				std::cout << dynamic_cast<IntDescriptor *>(parent)->intValue();
			else if (parent->type() == TypeDescriptor::DOUBLE)
				std::cout << dynamic_cast<DblDescriptor *>(parent)->doubleValue();
			else 
				std::cout << "\"" << dynamic_cast<StrDescriptor *>(parent)->stringValue() << "\"";
			std::cout << " ";
			if (!symTab.isDefined(parent))
				delete parent;
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

PrintStatement::~PrintStatement() {
	for (auto p : _testlist)
		delete p;
	_testlist.clear();
}

// For Statement 

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

	for (int i = 0; i < (int)_testlist.size(); i++) {
		TypeDescriptor* parent = _testlist[i]->evaluate(symTab);
		if (parent->type() != TypeDescriptor::INTEGER) {
			std::cout << "ForStatement::evaluate: parameters for range have to be integers\n";
			exit(1);
		}
		else {
			parameters.push_back(dynamic_cast<IntDescriptor*>(parent)->intValue());
		}
		if (!symTab.isDefined(parent))
			delete parent;
	}

	if (parameters.size() == 1)
		rg = new Range(parameters[0]);
	else if (parameters.size() == 2)
		rg = new Range(parameters[0], parameters[1]);
	else
		rg = new Range(parameters[0], parameters[1], parameters[2]);
	
	while (rg->condition()) {
		symTab.setValueFor(_id, new IntDescriptor(rg->current()));
		_stms->evaluate(symTab);
		rg->next();
	}
	delete rg;
	parameters.clear();
}

void ForStatement::print() {

	std::cout << "for " << _id << " in range(";	
	for (int i = 0; i < (int)_testlist.size() - 1; i++) {
		_testlist[i]->print();
		std::cout << ", ";
	}
	_testlist[_testlist.size() - 1]->print();
	std::cout << ") { \n";
	_stms->print();
	std::cout << "}\n";
}

ForStatement::~ForStatement() {
	for (auto p : _testlist)
		delete p;
	delete _stms;
	_testlist.clear();
	parameters.clear();
}

// IF Statement 
IfStatement::IfStatement() : testSuites{ 0 } {}

void IfStatement::evaluate(SymTab &symTab) {
	for (int i = 0; i < (int)testSuites.size(); i++) {
		if (dynamic_cast<IntDescriptor*>(testSuites[i].first->evaluate(symTab))->intValue()) {
			testSuites[i].second->evaluate(symTab);
			break;
		}
	}
}

void IfStatement::print() {
	std::cout << "if ";
	testSuites[0].first->print();
	std::cout << " :\n";
	testSuites[0].second->print();
	std::cout << std::endl;
	for (int i = 1; i < (int)testSuites.size(); i++) {
		std::cout << "elif ";
		testSuites[i].first->print();
		std::cout << " :\n";
		testSuites[i].second->print();
		std::cout << std::endl;
	}
}

void IfStatement::insertSuite(ExprNode* test, Statements* stms) {
	testSuites.push_back(std::make_pair(test, stms));
}

IfStatement::~IfStatement() {
	for (int i = 0; i < (int)testSuites.size(); i++) {
		delete testSuites[i].first;
		delete testSuites[i].second;
	}
	testSuites.clear();
}

// Array Statement 
ArrayStatement::ArrayStatement(ExprNode* test, std::string operation, std::string array) : _array{ array }, _operation{ operation }, _test{ test } {}

void ArrayStatement::evaluate(SymTab& symTab) {
	if (_operation == "append") {
		if (symTab.isDefined(_array)) {
			if (symTab.getValueFor(_array)->type() != TypeDescriptor::ARRAY) {
				std::cout << "\nVariable: " << _array << " is not an array";
				exit(1);
			}
			if (dynamic_cast<ArrDescriptor*>(symTab.getValueFor(_array))->type() != _test->evaluate(symTab)->type()) {
				std::cout << "\nYou cannot append an element of a different type to the elements within the array\n";
			}
			dynamic_cast<ArrDescriptor*>(symTab.getValueFor(_array))->testlist().push_back(_test->evaluate(symTab));
		}
	}
	else if (_operation == "pop") {
		if (symTab.isDefined(_array)) {
			if (symTab.getValueFor(_array)->type() != TypeDescriptor::ARRAY) {
				std::cout << "\nVariable: " << _array << " is not an array";
				exit(1);
			}
			dynamic_cast<ArrDescriptor*>(symTab.getValueFor(_array))->testlist().pop_back();
		}
	}
	else {
		std::cout << "\nOperation not supported on arrays: " << _operation << std::endl;
	}
}

void ArrayStatement::print() {
	std::cout << _array << "." << _operation << "(";
	if (_test != nullptr)
		_test->print();
	std::cout << ")";
}

ArrayStatement::~ArrayStatement() {
	delete _test;
}


// Function definition
FunctionStatement::FunctionStatement(std::string varName, std::vector<ExprNode*> params, Statements* suite) : _funcName{ varName }, _params { params }, _suite{ suite } {}

void FunctionStatement::evaluate(SymTab& symTab) {
	if (!symTab.isDefined(_funcName))
		symTab.setValueFor(_funcName, new FunDescriptor(_funcName, this));
}

void FunctionStatement::print() {
	std::cout << "def " << _funcName << "(";
	for (ExprNode* e : _params) {
		e->print();
		std::cout << ", ";
	}
	std::cout << "):\n";
	_suite->print();
}

std::vector<ExprNode*>& FunctionStatement::paramId() {
	return _params;
}

Statements*& FunctionStatement::suite() {
	return _suite;
}

FunctionStatement::~FunctionStatement() {
	for (int i = 0; i < (int)_params.size(); i++) {
		delete _params[i];
	}
	
	_params.clear();
	delete _suite;
}

// Function Descriptor 
FunDescriptor::FunDescriptor(std::string funcName, FunctionStatement* function) : TypeDescriptor(TypeDescriptor::FUNC), _funcName{ funcName }, _function{ function } {}
FunDescriptor::FunDescriptor(FunctionStatement* function) : TypeDescriptor(TypeDescriptor::FUNC), _function{ function } {}

FunctionStatement*& FunDescriptor::function() {
	return _function;
}

void FunDescriptor::print() {
	std::cout << _funcName << " contains a function type\n";
}

FunDescriptor::~FunDescriptor() {
	delete _function;
}

// Function Call Statement 
FunctionCallStatement::FunctionCallStatement(std::string funcName, std::vector<ExprNode*> args) : _funcName{ funcName }, _arguments{ args } {}

void FunctionCallStatement::evaluate(SymTab& symTab) {
	if (symTab.isDefined(_funcName)) {
		TypeDescriptor* parent = symTab.getValueFor(_funcName);
		if (parent->type() == TypeDescriptor::FUNC) {
			std::vector<ExprNode*> paramId = dynamic_cast<FunDescriptor*>(parent)->function()->paramId();
			Statements* suites = dynamic_cast<FunDescriptor*>(parent)->function()->suite();
			if (paramId.size() != _arguments.size()) {
				std::cout << "The number of parameters given for: " << _funcName << " is too much or too little\n";
				exit(1);
			}
			SymTab newSymTab = symTab;
			for (int i = 0; i < (int)paramId.size(); i++) {
				std::string paramString = paramId[i]->token().getName();
				newSymTab.setValueFor(paramString, _arguments[i]->evaluate(newSymTab));
			}
			suites->evaluate(newSymTab);
		}
		else {
			std::cout << "The variable name: " << _funcName << " is not defined for a function\n";
		}
	}
	else {
		std::cout << "The function: " << _funcName << " was not defined\n";
		exit(1);
	}
}

void FunctionCallStatement::print() {
	std::cout << _funcName << "(";
	for (ExprNode* e : _arguments) {
		e->print();
		std::cout << ", ";
	}
	std::cout << ")";	
}

FunctionCallStatement::~FunctionCallStatement() {
	for (ExprNode* e : _arguments)
		delete e;
	_arguments.clear();
}