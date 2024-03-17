% Лабораторная работа № 2.1. Синтаксические деревья
% 22 февраля 2024 г.
% Киселев Кирилл, ИУ9-61Б

# Цель работы

Целью данной работы является изучение представления
синтаксических деревьев в памяти компилятора
и приобретение навыков преобразования
синтаксических деревьев.

# Индивидуальный вариант

Динамические срезы, создаваемые функцией
make, должны иметь в два раза большую вместимость,
чем в исходной программе
(при этом их длина меняться не должна).

# Реализация

Демонстрационная программа:

```go
package main

import "fmt"

func main() {
	slice := make([]int, 10, 5+5)
	slice2 := make([]int, 10)
	mp := make(map[int]int, 10)

	fmt.Println(len(slice))
	fmt.Println(len(slice2))
	fmt.Println(len(mp))
}
```

Программа, осуществляющая преобразование
синтаксического дерева:

```go
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
```

# Тестирование

Результат трансформации демонстрационной программы:

```go
package main

import "fmt"

func main() {
	slice := make([]int, 10, 2*(5+5))
	slice2 := make([]int, 10, 2*10)
	mp := make(map[int]int, 10)

	fmt.Println(len(slice))
	fmt.Println(len(slice2))
	fmt.Println(len(mp))
}
```

# Вывод

В ходе выполнения лабораторной работы были получены
навыки работы с пакетами стандартной библиотеки golang,
позволяющими манипулировать синтаксическими деревьями.

Думаю, что полученные навыки можно было бы
использовать следующим образом:

- Генерация кода по шаблонам, например
[protoc](https://github.com/golang/protobuf/blob/v1.5.3/protoc-gen-go/generator/generator.go),
с помощью которого генерируются заглушки gRPC сервисов,
как раз использует пакеты для работы с синтаксическими
деревьями, чтобы генерировать код на golang
- Написания своего форматера, линтера, LSP
