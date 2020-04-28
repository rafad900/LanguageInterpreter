
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ArithExpr.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();
    Statements *suite();

    AssignmentStatement *assignStatement();
    PrintStatement *printStatement();
    ForStatement *forStatement();
    IfStatement* ifStatement();
    ArrayStatement* arrayStatement();

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();
	ExprNode *relExpr();
	ExprNode *relTerm();
	ExprNode *relPrimary();
	ExprNode *factor();
    ExprNode *test();
    ExprNode *testlist();
    ExprNode* or_test();
    ExprNode* and_test();
    ExprNode* not_test();
    ExprNode* subscription(ExprNode* variable, Token tok);

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

