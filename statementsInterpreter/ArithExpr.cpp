//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"
#include "Statements.hpp"

extern TypeDescriptor* perform_operation( TypeDescriptor* lValue, TypeDescriptor* rValue, int opcode);

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

ExprNode::~ExprNode() {};

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{ tk }, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

InfixExprNode::~InfixExprNode() {
    if (_left != nullptr)
        delete _left;
    if (_right != nullptr) 
        delete _right;
}

TypeDescriptor* InfixExprNode::evaluate(SymTab &symTab) {
    TypeDescriptor* lValue = nullptr;
    if (left() != nullptr)
        lValue = left()->evaluate(symTab);
    TypeDescriptor* rValue = right()->evaluate(symTab);
    TypeDescriptor* res;

    if( token().isAdditionOperator() ) {
    	res = perform_operation(lValue, rValue, 1);
    } else if(token().isSubtractionOperator()) {
		res = perform_operation(lValue, rValue, 2);
    } else if(token().isMultiplicationOperator()) {
		res = perform_operation(lValue, rValue, 3);
    } else if(token().isDivisionOperator()) {
    	if (token().isIntDivision()) {
			res = perform_operation(lValue, rValue, 12);
    	} else {
			res = perform_operation(lValue, rValue, 4);
		}
    } else if( token().isModuloOperator() ) {
		res = perform_operation(lValue, rValue, 11);
    } else if( token().isGreaterThan()) {
		res = perform_operation(lValue, rValue, 6);
    } else if( token().isLessThan()) {
		res = perform_operation(lValue, rValue, 5);
    } else if( token().isGreaterOrEqual()) {
    	res = perform_operation(lValue, rValue, 9);
    } else if( token().isLessOrEqual()) {
    	res = perform_operation(lValue, rValue, 8);
    } else if( token().isEqualTo()) {
    	res = perform_operation(lValue, rValue, 7);
    } else if( token().isNotEqualTo()) {
      	res = perform_operation(lValue, rValue, 10);
    } else if (token().isAnd()) {
        res = perform_operation(lValue, rValue, 13);
    } else if (token().isOr()) {
        res = perform_operation(lValue, rValue, 14);
    } else if (token().isNot()) {
        res = perform_operation(lValue, rValue, 15);
    } else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
    if (!symTab.isDefined(rValue))
        delete rValue;
    if (!symTab.isDefined(lValue))
        delete lValue;
    return res;
}

void InfixExprNode::print() {
    if (_left != nullptr)
        _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

TypeDescriptor* WholeNumber::evaluate(SymTab &symTab) {
    return new IntDescriptor(token().getWholeNumber());
}

WholeNumber::~WholeNumber() {}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor* Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "ERROR: Variable::evaluate: Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    return symTab.getValueFor(token().getName());
}

Variable::~Variable() {}

// UserString

UserString::UserString(Token token): ExprNode{token} {}

void UserString::print() {
	std::cout << "\"";
	token().print();
	std::cout << "\"";
}

TypeDescriptor* UserString::evaluate(SymTab &symTab) {
    return new StrDescriptor(token().getString());
}

UserString::~UserString() {}

// DoubleNumber

DoubleNumber::DoubleNumber(Token token): ExprNode{token} {}

void DoubleNumber::print() {
	token().print();
}

TypeDescriptor* DoubleNumber::evaluate(SymTab &symTab) {
    return new DblDescriptor(token().getDouble());
}

DoubleNumber::~DoubleNumber() {}

// Index Class 

Index::Index(Token token) : ExprNode{ token }, _id { nullptr }, _position{ nullptr } {}

ExprNode*& Index::id() { return _id; }

ExprNode*& Index::position() { return _position; }

void Index::print() {
    _id->print();
    std::cout << "[";
    _position->print();
    std::cout << "]";
}

TypeDescriptor* Index::evaluate(SymTab& symTab) {
    std::vector<TypeDescriptor*> array;

    if (symTab.isDefined(token().getName()))
        dynamic_cast<ArrDescriptor*>(symTab.getValueFor(token().getName()))->testlistCopy(array);

    if (_position->evaluate(symTab)->type() != TypeDescriptor::INTEGER) {
        std::cout << "\nThe index has to be the interger\n" << std::endl;
        exit(1);
    }

    int position = dynamic_cast<IntDescriptor*>( _position->evaluate(symTab))->intValue();
    return array[position];
}

Index::~Index() {}

// Len Class
Len::Len(Token token) : ExprNode(token) {}

void Len::print() {
    std::cout << "Len(" << token().getName() << ")";
}

TypeDescriptor* Len::evaluate(SymTab &symTab) {
    if (symTab.getValueFor(token().getName())->type() == TypeDescriptor::ARRAY) {
        int length = dynamic_cast<ArrDescriptor*>(symTab.getValueFor(token().getName()))->testlist().size();
        return new IntDescriptor(length);
    }
    else {
        std::cout << "The variable name: " << token().getName() << " is not a name for an array.";
        exit(1);
    }
}

Len::~Len() {}

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
}

// Call Class

Call::Call(Token token, std::vector<ExprNode*> args) : ExprNode(token), _arguments{ args } {}

TypeDescriptor* Call::evaluate(SymTab& symTab) {
    if (symTab.isDefined(token().getName())) {
        TypeDescriptor* parent = symTab.getValueFor(token().getName());
        if (parent->type() == TypeDescriptor::FUNC) {
            std::vector<ExprNode*> paramId = dynamic_cast<FunDescriptor*>(parent)->function()->paramId();
            Statements* suites = dynamic_cast<FunDescriptor*>(parent)->function()->suite();
            if (paramId.size() != _arguments.size()) {
                std::cout << "The number of parameters given for: " << token().getName() << " is too much or too little\n";
                exit(1);
            }
            SymTab newSymTab;
            newSymTab = symTab; // Using the operator overloaded function to make a deep copy of all the type descriptors in the map
            for (int i = 0; i < (int)paramId.size(); i++) {
                std::string paramString = paramId[i]->token().getName();
                newSymTab.setValueFor(paramString, _arguments[i]->evaluate(newSymTab));
            }
            suites->evaluate(newSymTab);

            if (newSymTab.isDefined("!{{FUNCTION_RETURN_VALUE}}!")) {
                TypeDescriptor* temp = newSymTab.getValueFor("!{{FUNCTION_RETURN_VALUE}}!");
                //newSymTab.delete_descriptors();
                return temp;
            }
            else
                //newSymTab.delete_descriptors();
                return nullptr;
        }
        else {
            std::cout << "The variable name: " << token().getName() << " is not defined for a function\n";
        }
    }
    else {
        std::cout << "The function: " << token().getName() << " was not defined\n";
        exit(1);
    }
    return nullptr;
}

void Call::print() {
    std::cout << token().getName() << "(";
    for (ExprNode* e : _arguments) {
        e->print();
        std::cout << ", ";
    }
    std::cout << ")";
}

Call::~Call() {
    for (ExprNode* e : _arguments)
        delete e;
    _arguments.clear();
}