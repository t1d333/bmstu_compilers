package lexer

import "fmt"

type Domain int

const (
	SET Domain = iota
	UNSET
	BRACKETS
	IDENT
	NUM
	STR
	EOP
)

type Token interface {
	GetType() Domain
	GetPos() (int, int)
	String() string
}

// SET token

type TokSet struct {
	pos [2]int
}

func (t TokSet) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokSet) GetType() Domain {
	return SET
}

func (t TokSet) String() string {
	return fmt.Sprintf("%v (%d, %d)", SET, t.pos[0], t.pos[1])
}

// UNSET token

type TokUnset struct {
	pos [2]int
}

func (t TokUnset) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokUnset) GetType() Domain {
	return UNSET
}

func (t TokUnset) String() string {
	return fmt.Sprintf("%v (%d, %d)", t.GetType(), t.pos[0], t.pos[1])
}

// BRACKETS token

type TokBrackets struct {
	pos [2]int
}

func (t TokBrackets) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokBrackets) GetType() Domain {
	return BRACKETS
}

func (t TokBrackets) String() string {
	return fmt.Sprintf("%v (%d, %d)", t.GetType(), t.pos[0], t.pos[1])
}

// IDENT token

type TokIdent struct {
	value string
	pos   [2]int
}

func (t TokIdent) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokIdent) GetType() Domain {
	return IDENT
}

func (t TokIdent) String() string {
	return fmt.Sprintf("%s (%d, %d): %s", t.GetType(), t.pos[0], t.pos[1], t.value)
}

// NUM token

type TokNum struct {
	value int64
	pos   [2]int
}

func (t TokNum) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokNum) GetType() Domain {
	return NUM
}

func (t TokNum) String() string {
	return fmt.Sprintf("%s (%d, %d): %d", t.GetType(), t.pos[0], t.pos[1], t.value)
}

// STR token

type TokStr struct {
	value string
	pos   [2]int
}

func (t TokStr) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokStr) GetType() Domain {
	return STR
}

func (t TokStr) String() string {
	return fmt.Sprintf("%s (%d, %d): %s", t.GetType(), t.pos[0], t.pos[1], t.value)
}

// EOP token

type TokEOP struct {
	pos [2]int
}

func (t TokEOP) GetPos() (int, int) {
	return t.pos[0], t.pos[1]
}

func (t TokEOP) GetType() Domain {
	return EOP
}

func (t TokEOP) String() string {
	return fmt.Sprintf("%s (%d, %d)", t.GetType(), t.pos[0], t.pos[1])
}

func (t Domain) String() string {
	switch t {
	case NUM:
		return "NUM"
	case IDENT:
		return "IDENT"
	case SET:
		return "SET"
	case UNSET:
		return "UNSET"
	case BRACKETS:
		return "BRACKETS"
	case STR:
		return "STR"
	case EOP:
		return "EOP"
	}

	return ""
}
