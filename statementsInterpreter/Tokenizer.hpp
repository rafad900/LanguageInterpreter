//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();
    int getnumoflines();

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens; 
    int numoflines = 0;
    std::vector<int> stack;
    bool parsingANewLine;
    int spaces;
    bool dedentagain;
    

private:
    void indent(Token &t);
    void dedent(Token &t);
	std::string readString( char p );
    std::string readName();
    std::string readNumber();
    void readComment();
};

#endif //EXPRINTER_TOKENIZER_HPP
