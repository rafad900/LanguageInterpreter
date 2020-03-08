//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
    void relationalSymbol(std::string s) { _relationalSymbol = s; }
    std::string relationalSymbol() { return _relationalSymbol; }
 	void keywordSymbol(std::string s) { _keyword = s; }
 	std::string keywordSymbol() const { return _keyword; }
	
	bool isPeriod() const { return _symbol == '.'; }
	bool isSingleQuotes() const { return _symbol == '\''; }
	bool isDoubleQuotes() const { return _symbol == '\"'; }
	bool isOpenBrac() const { return _symbol == '{'; }
	bool isCloseBrac() const { return _symbol == '}'; }
    bool isKeyword() const { return _name == "print" || _name == "for"; }
    bool isPrint() const { return _name == "print"; }
    bool isFor() const { return _name == "for"; }
	bool isLessThan() const 	{ return _symbol == '<'; }
	bool isGreaterThan() const 	{ return _symbol == '>'; }
	bool isEqualTo() const 		{ return _relationalSymbol == "=="; }
	bool isNotEqualTo() const	{ return _relationalSymbol == "!="; }
	bool isLessOrEqual() const	{ return _relationalSymbol == "<="; }
	bool isGreaterOrEqual() const	{return _relationalSymbol == ">="; }
	bool isRelationalOperator() const { 
		return	isLessThan() ||
				isGreaterThan() ||
				isEqualTo()		||
				isNotEqualTo()	||
				isLessOrEqual() 	||
				isGreaterOrEqual();
	}
    bool isSemiColon() const 	{ return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    bool isName() const         { return _name.length() > 0; }
    std::string getName() const { return _name; }
    void setName(std::string n) { _name = n; }
	void setString(std::string s) { _string = s; }

	bool isDouble() { return _double; }
	double getDouble() const { return _double; }
    bool &isWholeNumber() { return _isWholeNumber; }
    bool isString() const { return _string.length() > 0; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    std::string getString() const { return _string; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void print() const;

private:
	double _double;
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    std::string _keyword;
    int _wholeNumber;
    std::string _relationalSymbol;
	std::string _string;
};

#endif //EXPRINTER_TOKEN_HPP
