//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>
#include <utility>
#include "ArithExpr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
	virtual ~Statement();

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();
    virtual ~Statements();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    // You need the variable (the thing before the equal sign) and the expression (the thing after the equal sign) 
    AssignmentStatement(std::string lhsVar, int index, ExprNode *rhsExpr);
	AssignmentStatement(std::string lhsVar, std::vector<ExprNode*> rhsExpr);
	// Getters 
    std::string &lhsVariable();
	int& index();
	ExprNode *&rhsExpression();
	std::vector<ExprNode*> array();

    virtual void evaluate(SymTab &symTab);
    virtual void print();
	virtual ~AssignmentStatement();
	

	private:
	int _index;
    std::string _lhsVariable;
	std::vector<ExprNode*> _array;
    ExprNode *_rhsExpression;
};

class PrintStatement : public Statement {
	public:
		PrintStatement();
		PrintStatement( std::vector<ExprNode *> testlist);
		std::vector<ExprNode *> printString();

		virtual void evaluate(SymTab &symTab);
		virtual void print();
		virtual ~PrintStatement();

	private:
		std::vector<ExprNode *> _testlist;
};

class ForStatement : public Statement {
	public:
		ForStatement();
		ForStatement(std::string id, std::vector<ExprNode*> testlist, Statements *forStms);
		Statements *&stms();
		
		virtual void evaluate(SymTab &symTab);
		virtual void print();
		virtual ~ForStatement();

	private:
		std::string _id;
		std::vector<ExprNode*> _testlist;
		std::vector<int> parameters;
		Statements *_stms;
};

class IfStatement : public Statement {
public:
	IfStatement();

	virtual void evaluate(SymTab& symTab);
	virtual void print();
	void insertSuite(ExprNode* test, Statements* stms);
	virtual ~IfStatement();

private:
	std::vector<std::pair<ExprNode* , Statements*>> testSuites;
};

class ArrayStatement : public Statement {
public:
	ArrayStatement(ExprNode* test, std::string operation, std::string array);
	virtual void evaluate(SymTab& symTab);
	virtual void print();
	virtual ~ArrayStatement();
private:
	std::string _array;
	std::string _operation;
	ExprNode* _test;

};

class FunctionStatement : public Statement {
public:
	FunctionStatement(std::string varName, std::vector<ExprNode*> params, Statements* suite);
	virtual void evaluate(SymTab& SymTab);
	virtual void print();
	std::vector<ExprNode*>& paramId();
	Statements*& suite();
	virtual ~FunctionStatement();

private:
	std::string _funcName;
	std::vector<ExprNode*> _params;
	Statements* _suite;
};

class FunDescriptor : public TypeDescriptor {
public:
	FunDescriptor(std::string funcName, FunctionStatement* function);
	FunDescriptor(FunctionStatement* function);
	FunctionStatement*& function();
	virtual void print();
	virtual ~FunDescriptor();
private:
	FunctionStatement* _function;
	std::string _funcName;
};

class FunctionCallStatement : public Statement {
public:
	FunctionCallStatement(ExprNode* call);
	virtual void evaluate(SymTab& symTab);
	virtual void print();
	virtual ~FunctionCallStatement();
private:
	ExprNode* _call;
};

class ReturnStatement : public Statement {
public:
	ReturnStatement(ExprNode* test);
	virtual void evaluate(SymTab& SymTab);
	virtual void print();
	virtual ~ReturnStatement();
private:
	ExprNode* _test;
};
#endif //EXPRINTER_STATEMENTS_HPP
