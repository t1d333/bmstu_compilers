.PHONY: build-flex clean run

CC             			= g++ -g
LEX            			= flex -I 
BISON          			= bison
LEXER_SRC      			= ./flex/lexer.l
LEXER_OUT      			= ./syntax_analyzer/lexer/src/lexer.cpp
PARSER_SRC     			= ./bison/parser.y
PARSER_OUT      		= ./syntax_analyzer/parser/src/parser.cpp
PARSER_HEADER_OUT   = ./syntax_analyzer/parser/include/parser.h


build-flex:
	$(LEX) -o $(LEXER_OUT) $(LEXER_SRC)  
	
build-bison:
	$(BISON) -d --header=$(PARSER_HEADER_OUT) -o $(PARSER_OUT) $(PARSER_SRC)  
	
lexer: $(LEXER_SRC)
	$(LEX)  -+ $(LEXER_SRC) 
	$(CC)   lex.yy.cc -o lexer
	rm lex.yy.cc

build: build-bison build-flex clean
	mkdir -p build && cd build && cmake .. && cmake --build .

run:
	./build/syntax_analyzer/analyzer/analyzer

parser:

clean:
	rm -rf build
