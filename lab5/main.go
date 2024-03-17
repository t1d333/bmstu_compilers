package main

import (
	"fmt"
	"lab5/lexer"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Println("usage: ./main <filepath>")
		return
	}

	raw, _ := os.ReadFile(os.Args[1])

	program := string(raw)

	lex := lexer.NewLexer(lexer.NewAutomata(), program)

	for {
		tok := lex.Next()
		fmt.Println(tok)

		if tok.GetType() == lexer.EOP {
			return
		}
	}
}
