#include <iostream>
#include<vector>


#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
	// Just displays how to use it
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }
	// creates an input file stream
    std::ifstream inputStream;
	
	// This opens the input file and sets it to read 
    inputStream.open(argv[1], std::ios::in);
    if (!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }
	
	// Creates the tokenizer object from the file
    Tokenizer tokenizer(inputStream);
    // Creates the parser oject from the tokernizer
    Parser parser(tokenizer);
	// Create pointer to statements object from object returned from parser
    Statements *statements = parser.statements();
    Token lastToken = tokenizer.getToken();
	while (lastToken.eol()) 
		lastToken = tokenizer.getToken();
    // Checks for the end of file token, if not found, the it will say that it found unexpected token
    if ( !lastToken.eof()) {
        std::cout << "Unexpected token in input." << std::endl;
        lastToken.print();
        exit(1);
    }
    // Creates this symtab object
    SymTab symTab;
    std::cout << "\nABOUT TO PRINT\n";
    statements->print();
    std::cout << "ABOUT TO EVALUATE\n";
	statements->evaluate(symTab);
	// prints out the statements 

    // uses the symTab to evaluate the statements
    //statements->evaluate(symTab); BEFORE, THE EVALUATE STATEMENT WAS SECOND, I MADE IT FIRST
    // Something like pushing the tokens into the symboltable and the uses this to print the out
    std::cout << std::endl << "Symbol table contains the following variables.\n";
    symTab.print();
    std::cout << "Number of lines: " << tokenizer.getnumoflines() << std::endl;
    delete statements;
    return 0;
}
