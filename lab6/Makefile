
CC       = gcc -g
LEX      = flex -i -I 

numbers: numbers.lex
	$(LEX)  numbers.lex
	$(CC)   lex.yy.c -o numbers

lab: lab.lex
	$(LEX)  lab.lex
	$(CC)   lex.yy.c -o lab
