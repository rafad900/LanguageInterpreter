//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool& eof() { return _eof; }
    bool& eol() { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const { return _eol; }

    bool isOpenParen() const { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
    bool isOpenBrac() const { return _symbol == '{'; }
    bool isCloseBrac() const { return _symbol == '}'; }
    bool isOpenBracket() const { return _symbol == '['; }
    bool isCloseBracket() const { return _symbol == ']'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
    void relationalSymbol(std::string s) { _relationalSymbol = s; }
    std::string relationalSymbol() { return _relationalSymbol; }
    void keywordSymbol(std::string s) { _keyword = s; }
    std::string keywordSymbol() const { return _keyword; }

    void setIndent() { indent = true; }
    void setDedent() { dedent = true; }
    bool isIndent() const { return indent; }
    bool isDedent() const { return dedent; }
    bool isFunction() const { return _isFunction; }
    bool &setFunction() { return _isFunction; }

    bool isHash() const { return _symbol == '#'; }
    bool isPeriod() const { return _symbol == '.'; }
    bool isComma() const { return _symbol == ','; }
    bool isSingleQuotes() const { return _symbol == '\''; }
    bool isDoubleQuotes() const { return _symbol == '\"'; }

    bool isKeyword() const { return _name == "print" || _name == "for" || _name == "in" || _name == "range" || 
                                    _name == "and" || _name == "or" || _name == "not" || _name == "if"||
                                    _name == "else" || _name == "elif" || _name == "pop" || _name == "append"; }
    bool isAnd() const { return _name == "and"; }
    bool isOr() const { return _name == "or"; }
    bool isNot() const { return _name == "not"; }
    bool isIn() const { return _name == "in"; }
    bool isRange() const { return _name == "range"; }
    bool isPrint() const { return _name == "print"; }
    bool isFor() const { return _name == "for"; }
    bool isIf() const { return _name == "if"; }
    bool isElif() const { return _name == "elif"; }
    bool isElse() const { return _name == "else"; }
	bool isLessThan() const 	{ return _symbol == '<'; }
	bool isGreaterThan() const 	{ return _symbol == '>'; }
	bool isEqualTo() const 		{ return _relationalSymbol == "=="; }
	bool isNotEqualTo() const	{ return _relationalSymbol == "!=" || _relationalSymbol == "<>"; }
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
    bool isColon() const { return _symbol == ':'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isIntDivision() const 			  { return _isIntDivisionOp; }
    void setIntDivision() 				  { _isIntDivisionOp = true; } 
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
	
	bool &isDouble() { return _isDouble; }
	bool isDouble() const { return _isDouble; }
	double getDouble() const { return _double; }
	void setDouble(double d) { 
		_double = d;
		isDouble() = true;
	}

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
	
    std::string getString() const { return _string; }
    bool isString() const { return _string.length() > 0; }

    void print() const;

private:
    bool indent;
    bool dedent;
	double _double;
	bool _isDouble;
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    bool _isIntDivisionOp;
    std::string _keyword;
    int _wholeNumber;
    std::string _relationalSymbol;
	std::string _string;
    bool _isFunction;
};

#endif //EXPRINTER_TOKEN_HPP
