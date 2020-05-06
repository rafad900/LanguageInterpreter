//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include <algorithm>
#include "Tokenizer.hpp"

void Tokenizer::indent(Token &t) {
    parsingANewLine = false;
    if (stack[stack.size() - 1] < spaces) {
        t.setIndent();
        //std::cout << "This is the indent spaces: " << spaces << std::endl;
        stack.push_back(spaces);
    }
}

void Tokenizer::dedent(Token &t) {
    parsingANewLine = false;
    if (stack.size() > 1 && stack[stack.size() - 1] > spaces) {
        if (std::find(stack.begin(), stack.end(), spaces) != stack.end())
            stack.pop_back();
        else {
            std::cout << "There was an error in indentation\nHere are the tokens read up this point";
            printProcessedTokens();
            exit(1);
        }
        if (stack[stack.size() - 1] != spaces)
            dedentagain = true;
        t.setDedent();
    }
}

void Tokenizer::readComment() {
	char c;
	while ( inStream.get(c) && c != '\n') 
		;
	numoflines++;
}

std::string Tokenizer::readString( char p ) {
	std::string _string;
	char c;
	while( inStream.get(c) && (isalnum(c) || isspace(c) || ispunct(c) || c == '\'' || c == '\"') ) {
        if (c == '\n') {
            std::cout << "No closing quote on the string: " << _string << std::endl;
            exit(1);
        }
        if ((c == '\'' || c == '\"') && c != p) {
            std::cout << "Incorrect closing quote for the string: " << _string << std::endl;
            exit(1);
        }
        if (c == p) 
            break;
    	_string += c;
	}
    
	return _string;
}


std::string Tokenizer::readName() {
    std::string name;
    char c;
    while( inStream.get(c) && (isalnum(c) || c == '_')) {
        name += c;
    }
    if(inStream.good())  
        inStream.putback(c);
    return name;
}

bool isDouble(std::string s) {
	for (int i = 0; i < s.length(); i++)
		if (s[i] == '.')
			return true;
	return false;
}

std::string Tokenizer::readNumber() {

	std::string pre;
    char c;
    while( inStream.get(c) && (isdigit(c) || c == '.')) {
    	pre += c;
    }
    if(inStream.good())  
        inStream.putback(c);
    return pre;
}

// Here is the constructor
Tokenizer::Tokenizer(std::ifstream& stream) : ungottenToken{ false }, inStream{ stream }, lastToken{}, stack{ 0 }, parsingANewLine{ true }, spaces{ 0 }, dedentagain{ 0 } {}

Token Tokenizer::getToken() {
    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    if (dedentagain) {
        dedentagain = false;
        dedent(lastToken);
        return lastToken;
    }

    char c;
    char t;
    while (inStream.get(c) && isspace(c) && c != '\n') 
        spaces++;

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    Token token;
    if (inStream.eof()) {
        dedent(token);
        if (!token.isIndent() && !token.isDedent())
            token.eof() = true;
    }
    else if (c == '\n') {
        if (lastToken.eol())
            dedent(token);
        parsingANewLine = true;
        spaces = 0;
        numoflines++;
        if (!token.isIndent() && !token.isDedent())
            token.eol() = true;
    }
    else if (isdigit(c) || (c == '.' && isdigit(inStream.peek()))) {
        inStream.putback(c);
        std::string pre = readNumber();
        if (isDouble(pre)) {
            token.setDouble(std::stod(pre));
        }
        else {
            token.setWholeNumber(std::stoi(pre));
        }
    }
    else if (c == '=') {
        if (inStream.peek() == '=') {
            inStream.get(t);
            token.relationalSymbol(std::string(1, c) + t);
        }
        else {
            token.symbol(c);
        }
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        if (c == '/' && inStream.peek() == '/') {
            token.symbol(c);
            inStream.get(c);
            token.setIntDivision();
        }
        else {
            token.symbol(c);
        }
    }
    else if (c == '.')
        token.symbol(c);
    else if (c == ';')
        token.symbol(c);
    else if (c == ':')
        token.symbol(c);
    else if (c == '(' || c == ')')
        token.symbol(c);
    else if (c == '{')
        token.symbol(c);
    else if (c == '}')
        token.symbol(c);
    else if (c == ',')
        token.symbol(c);
    else if (c == ']')
        token.symbol(c);
    else if (c == '[')
        token.symbol(c);
    else if ( c == '\"' || c == '\'')
    	token.setString( readString(c) );
    else if(isalpha(c)) {
        inStream.putback(c);
        if (parsingANewLine) {
            indent(token);
            dedent(token);
        }
        if (!token.isDedent() && !token.isIndent()) {
            token.setName(readName());
            if (inStream.peek() == '.')
                token.setFunction() = true;
            else if (inStream.peek() == '(')
                token.setFunctionCall() = true;
        }
   	} else if( c == '>' || c == '<') {
   		if (inStream.peek() == '=') {
   			inStream.get(t);
   			token.relationalSymbol(std::string(1, c) + t);
   		} else token.symbol(c);
    } else if (c == '!') {
    	if (inStream.peek() == '=') {
    		inStream.get(t);
    		token.relationalSymbol(std::string(1, c) + t);
    	}
    } else if (c == '#') {
    	readComment();
    	token.eol() = true;
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
      	exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}

int Tokenizer::getnumoflines() {
	return numoflines;
}
