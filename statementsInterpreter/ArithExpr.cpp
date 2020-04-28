//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"

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
    TypeDescriptor* lValue;
    if (left() != nullptr)
        lValue = left()->evaluate(symTab);
    TypeDescriptor* rValue = right()->evaluate(symTab);
    TypeDescriptor* res;
    if (token().symbol() == '\0') {
        if (left() != nullptr) {
            std::cout << "InfixExprNode::evaluate: "; lValue->print(); std::cout << " " << token().relationalSymbol() << " "; rValue->print(); std::cout << std::endl;
        }
        else {
            std::cout << "InfixExprNode::evaluate: "; std::cout << token().relationalSymbol() << " "; rValue->print(); std::cout << std::endl;
        }
    }
    else {
        if (left() != nullptr) {
            std::cout << "InfixExprNode::evaluate: "; lValue->print(); std::cout << " " << token().symbol() << " "; rValue->print(); std::cout << std::endl;
        }
        else {
            std::cout << "InfixExprNode::evaluate: "; std::cout << token().relationalSymbol() << " "; rValue->print(); std::cout << std::endl;
        }
    }
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
    //std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
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
    //std::cout << "Variable::evaluate: returning " << std::endl;
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
    //std::cout << "UserString::evaluate: returning " << token().getString() << std::endl;
    return new StrDescriptor(token().getString());
}

UserString::~UserString() {}

// DoubleNumber

DoubleNumber::DoubleNumber(Token token): ExprNode{token} {}

void DoubleNumber::print() {
	token().print();
}

TypeDescriptor* DoubleNumber::evaluate(SymTab &symTab) {
    //std::cout << "Double::evaluate: returning " << token().getDouble() << std::endl;
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

