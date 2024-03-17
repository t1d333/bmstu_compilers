package task_test

import (
	"bytes"
	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/t1d333/compilers/task"
)

// var tests = map[string]struct{code: string }{}

func TestModify(t *testing.T) {
	tests := []struct {
		name   string
		code   string
		result string
	}{
		{
			name: "simple",
			code: `package main
			
			import "fmt"

			func main() {
				slice := make([]int, 5, 10)
				fmt.Println(len(slice), cap(slice))
			}`,

			result: `package main

import "fmt"

func main() {
	slice := make([]int, 5, 2*10)
	fmt.Println(len(slice), cap(slice))
}
`,
		},
	}

	buffer := bytes.NewBuffer([]byte{})

	for _, test := range tests {
		fset := token.NewFileSet()

		file, err := parser.ParseFile(
			fset,                 // данные об исходниках
			test.name+".go",      // имя файла с исходником программы
			test.code,            // пусть парсер сам загрузит исходник
			parser.ParseComments, // приказываем сохранять комментарии
		)
		assert.Nil(t, err, "Test: %s\nUnexpected error: %v\n", err, test.name)

		ast.Inspect(file, task.Modify)
		format.Node(buffer, fset, file)

		assert.Equal(t, test.result, buffer.String())
		buffer.Reset()
	}
}
