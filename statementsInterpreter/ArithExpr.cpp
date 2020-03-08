//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"
// #include <string> 

// Addition operations
std::string addition_of_string_and_string (std::string l, std::string r) {
    return l + r;
}

int addition_of_integer_and_integer(int l, int r) {
    return l + r;
}

double addition_of_integer_and_double(int i, double j) {
    return i + j;
}

double addition_of_double_and_double(double i, double j) {
    return i + j;
}

// Multiplication operations
std::string multiplication_of_string_and_integer(std::string s, int i) {
    for (int a = 0; a < i; a++) 
        s += s;
    return s;
}

int multiplication_of_integer_and_integer(int i, int j) {
    return i * j;
}

double multiplication_of_integer_and_double(double i, int j) {
    return i * j;
}

double multiplication_of_double_and_double(double i, double j) {
    return i * j;
}

// Subtraction operations
int subtraction_of_integer_and_integer(int i, int j) {
    return i - j;
}

double subtraction_of_integer_and_double(double i, int j, bool reverse) {
    if (reverse) return j - i;
    else return i - j;
}

double subtraction_of_double_and_double(double i, double j) {
    return i - j;
}

// Division operations
int division_of_integer_and_integer(int i, int j) {
    return i / j;
}

double division_of_integer_and_double(double i, int j, bool reverse) {
    if (reverse) return j / i;
    else return i / j;
}

double division_of_double_and_double(double i, double j) {
    return i / j;
}

TypeDescriptor* perform_operation( TypeDescriptor* lValue, TypeDescriptor* rValue, bool mul, bool div, bool add, bool sub) {
	if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING)) {		// STRING and STRING
		StringTypeDescriptor *left  = dynamic_cast<StringTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
       	if (add) return new StringTypeDescriptor(addition_of_string_and_string ( left->stringValue(), right->stringValue()));
        else { std::cout << "Only operation supported for string and string is addition\n"; exit(2);}
	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::STRING)) {  	// INTEGER and STRING
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
        if (mul) return new StringTypeDescriptor(multiplication_of_string_and_integer(right->stringValue(), left->intValue()));
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}
	} else if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::INTEGER)) { 	 // STRING and INTEGER
		StringTypeDescriptor *left   = dynamic_cast<StringTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (mul) return new StringTypeDescriptor(multiplication_of_string_and_integer(left->stringValue(), right->intValue()));
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}
	} else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::STRING) || 
                (lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::DOUBLE)) {  	// DOUBLE and STRING
       	std::cout << "No operation supported for string and double"; exit(2);
	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER)) {	// INTEGER and INTEGER
   		IntegerTypeDescriptor *left  = dynamic_cast<IntegerTypeDescriptor *>(lValue);
   		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (mul) return new IntegerTypeDescriptor(multiplication_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (div) return new IntegerTypeDescriptor(division_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (sub) return new IntegerTypeDescriptor(subtraction_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (add) return new IntegerTypeDescriptor(addition_of_integer_and_integer(left->intValue(), right->intValue()));
        else { std::cout << "Operation: not supported for integer and integer"; exit(2); }
    } else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::DOUBLE)) {	// INTEGER and DOUBLE 
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
        if (mul) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->intValue(), right->doubleValue()));
        else if (div) return new DoubleTypeDescriptor(division_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (sub) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (add) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->intValue(), right->doubleValue()));
        else { std::cout << "Operation: not supported for integer and double"; exit(2); }
    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::INTEGER)) {	// DOUBLE and INTEGER
		DoubleTypeDescriptor *left   = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
       	if (mul) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->doubleValue(), right->intValue()));
        else if (div) return new DoubleTypeDescriptor(division_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (sub) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (add) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->doubleValue(), right->intValue()));
        else { std::cout << "Operation: not supported for integer and double"; exit(2); }
    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::DOUBLE)) {	// DOUBLE and DOUBLE 
		DoubleTypeDescriptor *left  = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
       	if (mul) return new DoubleTypeDescriptor(multiplication_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (div) return new DoubleTypeDescriptor(division_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (sub) return new DoubleTypeDescriptor(subtraction_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (add) return new DoubleTypeDescriptor(addition_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else { std::cout << "Operation: not supported for double and double"; exit(2); }
	}
}

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
    TypeDescriptor *value = new IntegerTypeDescriptor(token().getName(), token().getWholeNumber());
    return value;
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor* Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
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
	if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    TypeDescriptor *value = symTab.getValueFor(token().getName());
    return value;
}

// DoubleNumber

DoubleNumber::DoubleNumber(Token token): ExprNode{token} {}

void DoubleNumber::print() {
	token().print();
}

TypeDescriptor* DoubleNumber::evaluate(SymTab &symTab) {
	if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    TypeDescriptor *value = symTab.getValueFor(token().getName());
    return value;
}
