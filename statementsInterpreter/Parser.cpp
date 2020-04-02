
#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <assignStatement> <statement> 
    // <statement> -> Epsilon

	// This does not grab tokens by themselves, it checks if its a name, and if it is then it will grab the whole line I assume
	// The assignStatement is one that actually grabs each of the tokens individually and gives them to the assingStmt object
    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while (tok.isName() || tok.eol()) {
		if (tok.eol()) {
    		tok = tokenizer.getToken();
    		continue;
    	}
    	if (tok.isKeyword()) {
    		if (tok.isFor()) {
				std::cout << "It has read for \n";
				tokenizer.ungetToken();
				ForStatement *forStmt = forStatement();
				stmts->addStatement(forStmt);
				tok = tokenizer.getToken();
    		} else if (tok.isPrint()) {
				std::cout << "IT has read print\n";
				tokenizer.ungetToken();
				PrintStatement *printStmt = printStatement();
				stmts->addStatement(printStmt);
				tok = tokenizer.getToken();
			}
    	} else {
    		std::cout << "It has read assignement\n";
			tokenizer.ungetToken();
			AssignmentStatement *assignStmt = assignStatement();
			stmts->addStatement(assignStmt);
			tok = tokenizer.getToken();
		}
    }
    tokenizer.ungetToken();
    return stmts;
}


PrintStatement *Parser::printStatement() {
	Token keyword = tokenizer.getToken();
	if (!keyword.isKeyword())
		die("Parser::printStatement", "Expected a keyword token, instead got", keyword); 
	
	std::vector<ExprNode *> testlist;
	Token test = tokenizer.getToken();
	while (test.isString() || test.isWholeNumber() || test.isName() || test.isComma() || test.isOpenParen() ) {
		if (test.isComma()) {
			test = tokenizer.getToken();
			continue;
		}
		tokenizer.ungetToken();
		testlist.push_back( relExpr() );	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
		test = tokenizer.getToken();
	}

	tokenizer.ungetToken();

	return new PrintStatement(testlist);
}


AssignmentStatement *Parser::assignStatement() {
    // Parses the following grammar rule
    // 
    // <assign-stmtement> -> <id> = <expr>

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);
	
	// This begins a sort of recursive call down the grammar rules 
    ExprNode *rightHandSideExpr = relExpr();
    // I removed the check for the semicolon and call the die function 
    Token tok = tokenizer.getToken();
    if (!tok.eol()) 
    	die("Parser::assignStatement", "Expected an equal sign, instead got", tok);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ForStatement *Parser::forStatement() {
	Token forkeyword = tokenizer.getToken();
	if (!forkeyword.isKeyword())
		die("Parser::forStatement", "Expected a for keyword token, instead got", forkeyword);
	
    Token id = tokenizer.getToken();
    if (!id.isName())
        die("Parser::forStatement", "Expected an id, instead got", id);

	Token inKeyword = tokenizer.getToken();
	if (!inKeyword.isKeyword())
		die("Parser::forStatement", "Expected a in keyword, instead got", inKeyword);

    Token rangeKeyword = tokenizer.getToken();
    if (!rangeKeyword.isKeyword())
        die("Parser::forStatement", "Expected a range keyword, instead got", rangeKeyword);

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::forStatement", "Expected a opening parentheses", openParen);


    std::vector<ExprNode*> testlist;
    Token test = tokenizer.getToken();
    while (test.isString() || test.isName() || test.isComma() || test.isOpenParen() || test.isWholeNumber()) {
        if (test.isComma()) {
            test = tokenizer.getToken();
            continue;
        }
        tokenizer.ungetToken();
        testlist.push_back(relExpr());	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
        test = tokenizer.getToken();
    }

    tokenizer.ungetToken();

	Token closeParen = tokenizer.getToken();
	if (!closeParen.isCloseParen())
		die("Parser::forstatement", "Expected a closing parentheses, instead got" , closeParen);
    
    Token possibleeol = tokenizer.getToken();
    if (possibleeol.eol()) 
    	while (possibleeol.eol()) 
    		possibleeol = tokenizer.getToken();
   
    tokenizer.ungetToken(); 
	
	Token colon = tokenizer.getToken();
	if (!colon.isColon())
		die("Parser::forstatement", "expected an open bracket, instead got", colon);

    Statements* stms = suite();
	
    return new ForStatement(id.getName() , testlist, stms);
}

IfStatement* Parser::ifStatement() {
    
    Token ifKeyword = tokenizer.getToken();
    if (!ifKeyword.isKeyword() && !ifKeyword.isIf())
        die("Parser::ifStatement", "Expected an if statement, instead got", ifKeyword);
    
    IfStatement* ifStatements = new IfStatement();
    ExprNode* _test = test();
    Token colon = tokenizer.getToken();
    if (!colon.isColon())
        die("Parser::ifStatement", "Expected a colon, instead got", colon);
    Statements* _stms = suite();
    ifStatements->insertSuite(_test, _stms);


    return ifStatements;
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.
	
    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::relExpr() {
	// Uses the grammar rule relexpr -> relterm {!=, == relterm}
    //ExprNode *left = relTerm();
    ExprNode *left = relPrimary();
    Token tok = tokenizer.getToken();
    while (tok.isEqualTo() || tok.isNotEqualTo() || tok.isLessThan() || tok.isGreaterThan() || tok.isLessOrEqual() || tok.isGreaterOrEqual() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        //p->right() = relTerm();
        p->right() = relPrimary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::factor() {	
	Token tok = tokenizer.getToken();
	ExprNode *left;
	if ( tok.isSubtractionOperator() ) {
		Token op;
		op.symbol('*');
		Token l;
		l.setWholeNumber(-1);
		InfixExprNode *p = new InfixExprNode(op);
		p->left() = new WholeNumber(l);
		p->right() = factor();
		left = p;
	} else {
		tokenizer.ungetToken();
		left = primary();
	}
	return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = factor();
    Token tok = tokenizer.getToken();
	// If there no more terms to look for, and only a semicolon, the while look will not return 
    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::relTerm() {
    ExprNode *left = relPrimary();
    Token tok = tokenizer.getToken();
    while (tok.isLessThan() || tok.isGreaterThan() || tok.isLessOrEqual() || tok.isGreaterOrEqual()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = relPrimary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
	else if ( tok.isDouble() )
    	return new DoubleNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if ( tok.isString() ) 
    	return new UserString(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = relExpr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);
    return nullptr;  // Will not reach this statement!
}

ExprNode *Parser::relPrimary() {
	ExprNode *singleArith = expr();
	return singleArith;
}

Statements* Parser::suite() {

    Token eol = tokenizer.getToken();
    if (!eol.eol())
        die("Parser::Suite", "Expected a new line, instead got", eol);

    Token indent = tokenizer.getToken();
    if (!indent.isIndent())
        die("Parser::Suite", "Expected an indent , instead got", indent);

    Statements* stms = statements();

    Token dedent = tokenizer.getToken();
    if (!dedent.isDedent())
        die("Parser::Suite", "Expected a dedent, instead got", dedent);

    return stms;
}



ExprNode* Parser::test() {
    
    ExprNode* left = or_test();
    Token tok = tokenizer.getToken();
    while (tok.isKeyword() && tok.isOr()) {
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = or_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode* Parser::and_test() {

    ExprNode* left = not_test();
    Token tok = tokenizer.getToken();
    while (tok.isKeyword() && tok.isNot()) {
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode* Parser::or_test() {
    ExprNode* left = and_test();
    Token tok = tokenizer.getToken();
    while (tok.isKeyword() && tok.isAnd()) {
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode* Parser::not_test() {
    Token tok = tokenizer.getToken();
    ExprNode* left;
    if (tok.isKeyword() && tok.isNot()) {
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = nullptr;
        p->right() = not_test();
        left = p;
    }
    else {
        tokenizer.ungetToken();
        left = relExpr();
    }
    return left;
}