//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"

extern TypeDescriptor* perform_operation( TypeDescriptor* lValue, TypeDescriptor* rValue, bool mul, bool div, bool add, bool sub);

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor* InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    TypeDescriptor* lValue = left()->evaluate(symTab);
    TypeDescriptor* rValue = right()->evaluate(symTab);
    
    if (token().symbol() == '\0') 
    	std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().relationalSymbol() << " " << rValue << std::endl;
    else std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    if( token().isAdditionOperator() )
		return perform_operation(lValue, rValue, 0, 0, 1, 0);
    else if(token().isSubtractionOperator())
		return perform_operation(lValue, rValue, 0, 0, 0, 1);
    else if(token().isMultiplicationOperator())
		return perform_operation(lValue, rValue, 1, 0, 0, 0);
    else if(token().isDivisionOperator())
		return perform_operation(lValue, rValue, 0, 1, 0, 0);
    /*else if( token().isModuloOperator() )
        return lValue % rValue;
    else if( token().isGreaterThan()) 
    	return lValue > rValue;
    else if( token().isLessThan()) 
    	return lValue < rValue;
    else if( token().isGreaterOrEqual()) 
    	return lValue >= rValue;
    else if( token().isLessOrEqual()) 
    	return lValue <= rValue;
    else if( token().isEqualTo()) 
    	return lValue == rValue;
    else if( token().isNotEqualTo()) 
    	return lValue != rValue;*/
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print() {
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
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    TypeDescriptor *value = new IntegerTypeDescriptor(token().getWholeNumber());
    return value;
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor* Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Variable::evaluate: Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    TypeDescriptor *value = symTab.getValueFor(token().getName());
    return value;
}

// UserString

UserString::UserString(Token token): ExprNode{token} {}

void UserString::print() {
	token().print();
}

TypeDescriptor* UserString::evaluate(SymTab &symTab) {
    std::cout << "UserString::evaluate: returning " << token().getString() << std::endl;
    TypeDescriptor *value = new StringTypeDescriptor(token().getString());
    return value;
}

// DoubleNumber

DoubleNumber::DoubleNumber(Token token): ExprNode{token} {}

void DoubleNumber::print() {
	token().print();
}

TypeDescriptor* DoubleNumber::evaluate(SymTab &symTab) {
    std::cout << "UserString::evaluate: returning " << token().getDouble() << std::endl;
    TypeDescriptor *value = new DoubleTypeDescriptor(token().getDouble());
    return value;
}
