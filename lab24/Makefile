.PHONY: build-flex clean run

CC             = g++ -g
LEX            = flex -I 
LEXER_SRC      = ./flex/lexer.l
LEXER_OUT      = ./syntax_analyzer/lexer/src/lexer.cpp


build-flex:
	$(LEX)  -+ -o $(LEXER_OUT) $(LEXER_SRC)  
	
lexer: $(LEXER_SRC)
	$(LEX)  -+ $(LEXER_SRC) 
	$(CC)   lex.yy.cc -o lexer
	rm lex.yy.cc

build: build-flex clean
	mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake  --build .

run:
	./build/syntax_analyzer/analyzer/analyzer



clean:
	rm -rf build
