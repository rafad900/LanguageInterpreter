.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects = TypeDescriptors.o Token.o Tokenizer.o Parser.o ArithExpr.o SymTab.o Statements.o main.o

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@


TypeDescriptors.o: TypeDescriptors.cpp TypeDescriptors.hpp
Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
ArithExpr.o: ArithExpr.cpp ArithExpr.hpp Token.hpp SymTab.hpp TypeDescriptors.hpp
SymTab.o: SymTab.cpp SymTab.hpp TypeDescriptors.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp ArithExpr.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp ArithExpr.hpp Token.hpp SymTab.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp ArithExpr.hpp Statements.hpp 


clean:
	rm -fr *.o *~ *.x
