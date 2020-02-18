//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _relationalSymbol{"\0"}, _keyword{"\0"} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isLessThan() ) std::cout << " < " ;
    else if( isGreaterThan() ) std::cout << " > " ;
    else if( isEqualTo() ) std::cout << " == " ;
    else if( isNotEqualTo() ) std::cout << " != " ;
    else if( isLessOrEqual() ) std::cout << " <= " ;
    else if( isGreaterOrEqual() ) std::cout << " >= " ;
    else if( isKeyword() ) std::cout << keywordSymbol();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isOpenBrac() ) std::cout << " { ";
   	else if( isCloseBrac() ) std::cout << " } ";
	else if( isPrint() ) std::cout << " print ";
	else if( isFor() ) std::cout << " for ";
    else std::cout << "Uninitialized token.\n";
}
