package lexer

import (
	"unicode"
)

type Automata interface {
	MakeTransition(state int, c rune) int
	IsFinal(state int) bool
	MarkupFunc(state int) (Domain, bool)
}

type LexerAutomata struct {
	table       [15][11]int
	finalStates map[int]struct{}
	markupTable map[int]Domain
}

func NewAutomata() Automata {
	return &LexerAutomata{
		table: [15][11]int{
			/*                      s  e  t  u  n  (   )  Lt Nm   ' ',  Other        */
			/*     start state  */ {2, 8, 8, 1, 8, 9, -1, 8, 13, -1, -1},
			/*     state 1      */ {8, 8, 8, 8, 14, -1, -1, 8, 8, -1, -1},
			/*     state 2      */ {8, 3, 8, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 3      */ {8, 8, 4, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 4      */ {8, 8, 8, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 5      */ {8, 6, 8, 8, 8, -1, -1, 8, 9, -1, -1},
			/*     state 6      */ {8, 8, 7, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 7      */ {8, 8, 8, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 8      */ {8, 8, 8, 8, 8, -1, -1, 8, 8, -1, -1},
			/*     state 9      */ {12, 12, 12, 12, 12, -1, 10, 12, 12, 12, -1},
			/*     state 10     */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			/*     state 11     */ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			/*     state 12     */ {12, 12, 12, 12, -1, -1, 11, 12, 12, 12, 12},
			/*     state 13     */ {-1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1},
			/*     state 14     */ {5, 8, 8, 8, 8, -1, -1, 8, 8, -1, -1},
		},

		finalStates: map[int]struct{}{
			1:  {},
			2:  {},
			3:  {},
			4:  {},
			5:  {},
			6:  {},
			7:  {},
			8:  {},
			10: {},
			11: {},
			13: {},
			14: {},
		},
		markupTable: map[int]Domain{
			1:  IDENT,
			2:  IDENT,
			3:  IDENT,
			4:  SET,
			5:  IDENT,
			6:  IDENT,
			7:  UNSET,
			8:  IDENT,
			10: BRACKETS,
			11: STR,
			13: NUM,
			14: IDENT,
		},
	}
}

func (a *LexerAutomata) MakeTransition(state int, c rune) int {
	switch {
	case c == 's':
		return a.table[state][0]
	case c == 'e':
		return a.table[state][1]
	case c == 't':
		return a.table[state][2]
	case c == 'u':
		return a.table[state][3]
	case c == 'n':
		return a.table[state][4]
	case c == '(':
		return a.table[state][5]
	case c == ')':
		return a.table[state][6]
	case unicode.IsLetter(c):
		return a.table[state][7]
	case unicode.IsDigit(c):
		return a.table[state][8]
	case unicode.IsSpace(c) && c != ' ' :
		return a.table[state][9]
	default:
		return a.table[state][10]
	}
}

func (a *LexerAutomata) IsFinal(state int) bool {
	_, ok := a.finalStates[state]

	return ok
}

func (a *LexerAutomata) MarkupFunc(state int) (Domain, bool) {
	tok, ok := a.markupTable[state]

	return tok, ok
}
