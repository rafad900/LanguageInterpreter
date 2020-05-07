#include <iostream>
#include <vector>
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
    inputStream.close();
    // Checks for the end of file token, if not found, the it will say that it found unexpected token
    if ( !lastToken.eof()) {
        std::cout << "Unexpected token in input." << std::endl;
        lastToken.print();
        exit(1);
    }
    // Creates this symtab object
    SymTab symTab;
    //std::cout << "\nABOUT TO PRINT\n";
    //statements->print();
	statements->evaluate(symTab);

    // uses the symTab to evaluate the statements
    delete statements;
    symTab.delete_descriptors();
    return 0;
}
