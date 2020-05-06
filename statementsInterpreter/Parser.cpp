
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
            else if (tok.isdef()) {
                tokenizer.ungetToken();
                FunctionStatement* funcStmt = functionStatement();
                stmts->addStatement(funcStmt);
                tok = tokenizer.getToken();
            }
            else {
                tokenizer.ungetToken();
                break;
            }
    	}
        else if (tok.isName() && tok.isArrayFunction()) {
            std::cout << "It has read an array statement";
            tokenizer.ungetToken();
            ArrayStatement* arrayStmt = arrayStatement();
            stmts->addStatement(arrayStmt);
            tok = tokenizer.getToken();
        }
        else if (tok.isName() && tok.isFuntionCall()) {
            std::cout << "It has read a function call statement";
            tokenizer.ungetToken();
            FunctionCallStatement* funcCallStmt = functioncallStatement();
            stmts->addStatement(funcCallStmt);
            tok = tokenizer.getToken();
        }
        else if (tok.isName()) {
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

    bool isSubscription = false;
    Token possibleBracket = tokenizer.getToken();
    Token index;
    if (possibleBracket.isOpenBracket()) {
        isSubscription = true;
        index = tokenizer.getToken();
        possibleBracket = tokenizer.getToken();
        if (!possibleBracket.isCloseBracket())
            die("Parser::AssignmentStatement", "Expecting a closing square bracket, instead got", possibleBracket);
    }
    else {
        tokenizer.ungetToken();
    }

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    bool isTestList = false;
    possibleBracket = tokenizer.getToken();
    if (possibleBracket.isOpenBracket() && isSubscription == false) {
        std::vector<ExprNode*> testlist;
        Token test_token = tokenizer.getToken();
        while (test_token.isString() || test_token.isName() || test_token.isComma() || test_token.isOpenParen() || test_token.isWholeNumber() || test_token.isDouble()) {
            if (test_token.isComma()) {
                test_token = tokenizer.getToken();
                continue;
            }
            tokenizer.ungetToken();
            testlist.push_back(test());	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
            test_token = tokenizer.getToken();
        }
        if (!test_token.isCloseBracket())
            die("Parser::AssignmentStatement", "Expected a closing bracket, instead got", possibleBracket);
        return new AssignmentStatement(varName.getName(), testlist);
    }
    tokenizer.ungetToken();
	// This begins a sort of recursive call down the grammar rules 
    ExprNode *rightHandSideExpr = test();
    // I removed the check for the semicolon and call the die function 
    Token tok = tokenizer.getToken();
    if (!tok.eol()) 
    	die("Parser::assignStatement", "Expected an equal sign, instead got", tok);
    
    if (isSubscription) {
        return new AssignmentStatement(varName.getName(), index.getWholeNumber(), rightHandSideExpr);
    }

    return new AssignmentStatement(varName.getName(), -1, rightHandSideExpr);
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
    while (test_token.isString() || test_token.isName() || test_token.isComma() || test_token.isOpenParen() || test_token.isWholeNumber() || test_token.isDouble()) {
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

ArrayStatement* Parser::arrayStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::ArrayStatement", "Expected a variable name, instead got: ", varName);

    Token dotNote = tokenizer.getToken();
    if (!dotNote.isPeriod())
        die("Parser::ArrayStatement", "Expected a period, instead got: ", dotNote);

    Token function = tokenizer.getToken();
    if (!function.isKeyword())
        die("Parser::ArrayStatement", "Expected an array operation, instead got: ", function);

    Token openParentheses = tokenizer.getToken();
    if (!openParentheses.isOpenParen())
        die("Parser::ArrayStatement", "Expected an open parentheses, instead got: ", openParentheses);

    ExprNode* _test = nullptr;
    if (function.getName() == "append")
        _test = test();
    
    Token closeParentheses = tokenizer.getToken();
    if (!closeParentheses.isCloseParen())
        die("Parser::ArrayStatement", "Expected a closed paretheses, instead got: ", closeParentheses);

    return new ArrayStatement(_test, function.getName(), varName.getName());
}

FunctionStatement* Parser::functionStatement () {
    Token def = tokenizer.getToken();
    if (!def.isdef())
        die("Parser::functionStatement", "def keyword expected, instead got", def);

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::functionStatement", "Expected a variable name, instead got", varName);

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::functionStatement", "Expected an open paretheses, instead got", openParen);

    std::vector<ExprNode*> parameters;
    Token test_token = tokenizer.getToken();
    while ( test_token.isName() || test_token.isComma() || test_token.isOpenParen()) {
        if (test_token.isComma()) {
            test_token = tokenizer.getToken();
            continue;
        }
        tokenizer.ungetToken();
        parameters.push_back(test());	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
        test_token = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::functionStatement", "Expected a closing parentheses, instead got", closeParen);

    Token colon = tokenizer.getToken();
    if (!colon.isColon())
        die("Parser::functionStatement", "Expected a colon, instead got", colon);

    Statements* stms = suite();


    return new FunctionStatement(varName.getName(), parameters, stms);
}

FunctionCallStatement* Parser::functioncallStatement() {
    Token funcName = tokenizer.getToken();
    if (!funcName.isName())
        die("Parser::FunctionCallStatement", "Expected a function name, instead got", funcName);

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::FunctionCallStatement", "Expected an open parentheses, instead got", openParen);

    std::vector<ExprNode*> arguments;
    Token test_token = tokenizer.getToken();
    while (test_token.isName() || test_token.isWholeNumber() || test_token.isDouble() || test_token.isString() || test_token.isComma() || test_token.isOpenParen()) {
        if (test_token.isComma()) {
            test_token = tokenizer.getToken();
            continue;
        }
        tokenizer.ungetToken();
        arguments.push_back(test());	// MAKE ALL THE NEW EXPRESSION NODES AND SAVE THEM INTO THE VECTOR 
        test_token = tokenizer.getToken();
    }
    tokenizer.ungetToken();

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::FunctionCallStatement", "Expected a closing parentheses, instead got", closeParen);

    return new FunctionCallStatement(funcName.getName(), arguments);
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

ExprNode* Parser::factor() {
    Token tok = tokenizer.getToken();
    ExprNode* left = nullptr;

    if (!tok.isSubtractionOperator()) {
        tokenizer.ungetToken();
        left = primary(); 
        left = subscription(left, tok);
    }

    if (tok.isSubtractionOperator()) {
        Token op;
        op.symbol('*');
        Token l;
        l.setWholeNumber(-1);
        InfixExprNode* p = new InfixExprNode(op);
        p->left() = new WholeNumber(l);
        p->right() = factor();
        left = p;
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

    if (tok.isWholeNumber())
        return new WholeNumber(tok);
    else if (tok.isDouble())
        return new DoubleNumber(tok);
    else if (tok.isName()) {                 // Add the subscription logic here 
        if (tok.isLen())
            return lenOperation();
        return new Variable(tok);
    }
    else if (tok.isString())
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

ExprNode* Parser::subscription(ExprNode *variable, Token tok) {
    Token possibleBracket = tokenizer.getToken();
    if (possibleBracket.isOpenBracket()) {
        ExprNode* position = test();
        Index* subscription = new Index(tok);
        subscription->position() = position;
        subscription->id() = variable;
        possibleBracket = tokenizer.getToken();
        if (!possibleBracket.isCloseBracket())
            die("Parser::primary", "Expected a closing bracket instead got", possibleBracket);
        return subscription;
    }
    else {
        tokenizer.ungetToken();
        return variable;
    }
}

ExprNode* Parser::lenOperation() {

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::LenOperation", "Expected an open parentheses, instead got", openParen);
    
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::LenOPeration", "Expected an array variable name, instead got", varName);

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::LenOPeration", "Expected a closing parentheses, instead got", closeParen);

    return new Len(varName);
}
