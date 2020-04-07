
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
				//std::cout << "It has read for \n";
				tokenizer.ungetToken();
				ForStatement *forStmt = forStatement();
				stmts->addStatement(forStmt);
				tok = tokenizer.getToken();
    		} else if (tok.isPrint()) {
				//std::cout << "It has read print\n";
				tokenizer.ungetToken();
				PrintStatement *printStmt = printStatement();
				stmts->addStatement(printStmt);
				tok = tokenizer.getToken();
            }
            else if (tok.isIf()) {
                //std::cout << "It has read if\n";
                tokenizer.ungetToken();
                IfStatement* ifStmt = ifStatement();
                stmts->addStatement(ifStmt);
                tok = tokenizer.getToken();
            }
            else {
                tokenizer.ungetToken();
                break;
            }
    	} else {
    		//std::cout << "It has read assignement\n";
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
	Token test_token = tokenizer.getToken();
	while (test_token.isString() || test_token.isWholeNumber() || test_token.isName() || test_token.isComma() || test_token.isOpenParen() ) {
		if (test_token.isComma()) {
			test_token = tokenizer.getToken();
			continue;
		}
		tokenizer.ungetToken();
		testlist.push_back( test() );	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
		test_token = tokenizer.getToken();
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
    ExprNode *rightHandSideExpr = test();
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
    Token test_token = tokenizer.getToken();
    while (test_token.isString() || test_token.isName() || test_token.isComma() || test_token.isOpenParen() || test_token.isWholeNumber()) {
        if (test_token.isComma()) {
            test_token = tokenizer.getToken();
            continue;
        }
        tokenizer.ungetToken();
        testlist.push_back(test());	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
        test_token = tokenizer.getToken();
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

    Token elseOrElif = tokenizer.getToken();
    if (elseOrElif.eol())
        while (elseOrElif.eol())
            elseOrElif = tokenizer.getToken();

    while (elseOrElif.isKeyword() || elseOrElif.eol()) {
        if (elseOrElif.isElif()) {
            ExprNode* next_test = test();
            Token colon = tokenizer.getToken();
            Statements* next_stms = suite();
            ifStatements->insertSuite(next_test, next_stms);
            elseOrElif = tokenizer.getToken();
        }
        else if (elseOrElif.isElse()) {
            Token one;
            one.setWholeNumber(1);
            ExprNode* next_test = new WholeNumber(one);
            Token colon = tokenizer.getToken();
            Statements* next_stms = suite();
            ifStatements->insertSuite(next_test, next_stms);
            break;
        }
        else if (elseOrElif.eol()) {
            elseOrElif = tokenizer.getToken();
        }
        else {
            tokenizer.ungetToken();
            break;
        }
    }

    return ifStatements;
}

ExprNode *Parser::expr() {
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
    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isEqualTo() || tok.isNotEqualTo() || tok.isLessThan() || tok.isGreaterThan() || tok.isLessOrEqual() || tok.isGreaterOrEqual() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
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

    ExprNode *left = factor();
    Token tok = tokenizer.getToken();
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

ExprNode *Parser::primary() {

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
    return left;
}

ExprNode* Parser::or_test() {
    
    ExprNode* left = and_test();
    Token tok = tokenizer.getToken();
    while (tok.isKeyword() && tok.isOr()) {
        tok.relationalSymbol("or");
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode* Parser::and_test() {
    ExprNode* left = not_test();
    Token tok = tokenizer.getToken();
    while (tok.isKeyword() && tok.isAnd()) {
        tok.relationalSymbol("and");
        InfixExprNode* p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
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