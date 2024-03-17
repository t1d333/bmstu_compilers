package main

import (
	"bufio"
	"fmt"
	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
	"os"

	"github.com/t1d333/compilers/task"
)

func main() {
	in := bufio.NewReader(os.Stdout)
	out := bufio.NewWriter(os.Stdin)

	defer out.Flush()

	fileName := ""
	// modifiedFileName := ""

	fmt.Fscan(in, &fileName)

	fset := token.NewFileSet()

	// Вызываем парсер
	if file, err := parser.ParseFile(
		fset,                 // данные об исходниках
		fileName,             // имя файла с исходником программы
		nil,                  // пусть парсер сам загрузит исходник
		parser.ParseComments, // приказываем сохранять комментарии
	); err != nil {
	} else {
		ast.Inspect(file, task.Modify)
		format.Node(out, fset, file)
	}
}
