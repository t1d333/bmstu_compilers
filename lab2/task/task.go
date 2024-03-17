package task

import (
	"go/ast"
	"go/token"
)

func Modify(node ast.Node) bool {
	callExpr, ok := node.(*ast.CallExpr)

	if !ok {
		return true
	}

	if ident, ok := callExpr.Fun.(*ast.Ident); !ok || ident.Name != "make" {
		return true
	}

	if _, ok := callExpr.Args[0].(*ast.ArrayType); !ok {
		return true
	}

	switch len(callExpr.Args) {

	case 2:

		lenArg := callExpr.Args[1]

		callExpr.Args = append(callExpr.Args, &ast.BinaryExpr{
			X:  &ast.BasicLit{ValuePos: 0, Kind: token.INT, Value: "2"},
			Op: token.MUL,
			Y:  lenArg,
		})

	case 3:

		capArg := callExpr.Args[2]

		callExpr.Args[2] = &ast.BinaryExpr{
			X: &ast.BasicLit{
				ValuePos: 0,
				Kind:     token.INT,
				Value:    "2",
			},
			Op: token.MUL,
			Y:  capArg,
		}

	}

	return true
}
