//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _isDouble{false}, _name{"\0"}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false},
    _relationalSymbol{"\0"}, _keyword{"\0"}, _string{"\0"}, _double{false}, _isIntDivisionOp{false},
    _wholeNumber{ false }, indent{ false }, dedent{ false }, _isFunction{ false }, _isArray{ false } {}

void Token::print() const {
    if (eol()) std::cout << "EOL\n";
    else if (eof())                    std::cout << "EOF";
    else if (isOpenParen())  std::cout << "(";
    else if (isCloseParen())  std::cout << ")";
    else if (isAssignmentOperator())  std::cout << " = ";
    else if (isSemiColon())  std::cout << ";";
    else if (isMultiplicationOperator())  std::cout << " * ";
    else if (isAdditionOperator())  std::cout << " + ";
    else if (isSubtractionOperator())  std::cout << " - ";
    else if (isModuloOperator())  std::cout << " % ";
    else if (isDivisionOperator() && isIntDivision()) std::cout << " // ";
    else if (isDivisionOperator())  std::cout << " / ";
    else if (isName())  std::cout << getName();
    else if (isLessThan()) std::cout << " < ";
    else if (isGreaterThan()) std::cout << " > ";
    else if (isEqualTo()) std::cout << " == ";
    else if (isNotEqualTo()) std::cout << " != ";
    else if (isLessOrEqual()) std::cout << " <= ";
    else if (isGreaterOrEqual()) std::cout << " >= ";
    else if (isKeyword()) std::cout << keywordSymbol();
    else if (isWholeNumber()) std::cout << getWholeNumber();
    else if (isOpenBrac()) std::cout << " { ";
    else if (isCloseBrac()) std::cout << " } ";
    else if (isPrint()) std::cout << " print ";
    else if (isFor()) std::cout << " for ";
    else if (isString()) std::cout << getString();
    else if (isHash()) std::cout << " # ";
    else if (isDouble()) std::cout << getDouble();
    else if (isComma()) std::cout << " , ";
    else if (isIndent()) std::cout << "INDENT ";
    else if (isDedent()) std::cout << "DEDENT ";
    else if (isAnd()) std::cout << " and ";
    else if (isOr()) std::cout << " or ";
    else if (isNot()) std::cout << " not ";
    else if (isColon()) std::cout << " : ";
    else if (isOpenBracket()) std::cout << "[";
    else if (isCloseBracket()) std::cout << "]";
    else if (isPeriod()) std::cout << ".";
    else std::cout << "Uninitialized token.\n";
}
