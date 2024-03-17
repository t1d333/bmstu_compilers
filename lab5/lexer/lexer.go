package lexer

import (
	"fmt"
	"strconv"
	"unicode"
)

type Lexer struct {
	automata Automata
	program  []rune
	pos      [2]int
	cur      int
	recovery bool
}

func NewLexer(a Automata, program string) Lexer {
	return Lexer{
		automata: a,
		program:  []rune(program),
		pos:      [2]int{1, 1},
		cur:      0,
		recovery: false,
	}
}

func (l *Lexer) Next() Token {
	for {
		for l.cur < len(l.program) && unicode.IsSpace(l.program[l.cur]) {

			l.pos = l.updatePos(l.pos)
			l.cur += 1

		}

		if l.cur >= len(l.program) {
			return TokEOP{pos: [2]int{l.pos[0], l.pos[1]}}
		}

		idxStart := l.cur
		pos := l.pos
		lastFinalState := -1
		lastFinalIdx := -1
		attr := []rune{}
		currState := 0

		for l.cur < len(l.program) {

			currState = l.automata.MakeTransition(currState, l.program[l.cur])

			if l.automata.IsFinal(currState) {
				lastFinalState = currState
				lastFinalIdx = l.cur
			}

			if currState == -1 {
				break
			}

			pos = l.updatePos(pos)

			attr = append(attr, l.program[l.cur])

			l.cur += 1
		}

		if lastFinalState != -1 {
			l.recovery = false

			tokType, _ := l.automata.MarkupFunc(lastFinalState)
			l.cur = lastFinalIdx + 1

			start := l.pos
			l.pos = pos

			switch tokType {
			case SET:
				return TokSet{pos: start}
			case UNSET:
				return TokUnset{pos: start}
			case IDENT:
				return TokIdent{pos: start, value: string(attr)}
			case NUM:
				value, _ := strconv.ParseInt(string(attr), 10, 64)
				return TokNum{pos: start, value: value}
			case STR:
				return TokStr{pos: start, value: string(attr[1 : len(attr)-2])}
			case BRACKETS:
				return TokBrackets{pos: start}
			}
		} else {
			if !l.recovery {
				l.recovery = true
				fmt.Printf("SYNTAX ERROR at (%d, %d)\n", l.pos[0], l.pos[1])

			}
			l.cur = idxStart + 1
			l.pos = l.updatePos(l.pos)
			if l.program[l.cur] == '\n' {
				l.pos[0] -= 1
			}

			return l.Next()
		}
	}
}

func (l *Lexer) updatePos(pos [2]int) [2]int {
	if l.program[l.cur] == '\n' {
		pos[0] += 1
		pos[1] = 1
	} else {
		pos[1] += 1
	}
	return pos
}
