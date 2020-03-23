//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

void Tokenizer::readComment() {
	char c;
	while ( inStream.get(c) && c != '\n') 
		;
}

std::string Tokenizer::readString( char p ) {
	// This function is called when the character read 
	// is an open quote character signifying the beginning
	// of a string
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
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
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
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.
	std::string pre;
    char c;
    while( inStream.get(c) && (isdigit(c) || c == '.')) {
    	pre += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return pre;
}

// Here is the constructor
Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {
    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    char t;
    while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
        ;

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }
	
    Token token;
    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
    	numoflines++;
        token.eol() = true;
    } else if( isdigit(c) || c == '.') {
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
		std::string pre = readNumber();
		if (isDouble(pre)) {
			token.setDouble( std::stod(pre) );
		} else { 
			token.setWholeNumber( std::stoi(pre) );
		}
    } else if( c == '=' ) {
    	if (inStream.peek() == '=') {
    		inStream.get(t);
    		token.relationalSymbol(std::string(1, c) + t);		
        } else {
        	token.symbol(c);
        }
    } else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
    	if ( c == '/' && inStream.peek() == '/' ) {
    		token.symbol(c);
    		inStream.get(c);
    		token.setIntDivision();
        } else {
        	token.symbol(c);
        }
    } else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if ( c == '{' )
    	token.symbol(c);
    else if ( c == '}' )
    	token.symbol(c);
   	else if ( c == ',' )
   		token.symbol(c);
    else if ( c == '\"' || c == '\'')
    	token.setString( readString(c) );
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
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
