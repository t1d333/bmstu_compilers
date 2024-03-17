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
