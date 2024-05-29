import parser_edsl as pe
from pprint import pprint
from dataclasses import dataclass
import sys
import abc

# GRAMMAR
# Start -> Func Start | .
# Func -> IDENT { FuncBody }
# FuncBody -> Sentence FuncBody | .
# Sentence -> Lhs = Result; | Lhs, Result : { FuncBody };
# Lhs -> Pattern | Lhs, Condition
# Condition -> Result : Pattern
# Result -> ResultPart Result | eps
# ResultPart -> INDENT | 'STRING' | INTEGER | VAR | (Result) | < IDENT Result >
# Pattern -> PatternPart Pattern | eps
# PatternPart ->  INDENT | 'STRING' | INTEGER | VAR | (Pattern)


@dataclass
class ResultPart(abc.ABC):
    pass


@dataclass
class ResultPartSimple:
    value: str


@dataclass
class Result:
    parts: list[ResultPart]


@dataclass
class ResultPartNested:
    result: Result


@dataclass
class PatternPart(abc.ABC):
    pass


@dataclass
class PatternPartSimple(PatternPart):
    value: str


@dataclass
class ResultPartFunc(PatternPart):
    ident: str
    result: Result


@dataclass
class Pattern:
    parts: list[PatternPart]


@dataclass
class PatternPartNested:
    pattern: Pattern


@dataclass
class Condition:
    result: Result
    pattern: Pattern


@dataclass
class Lhs:
    pattern: Pattern
    conditions: list[Condition]


@dataclass
class Sentence:
    lhs: Lhs


@dataclass
class SentenceResult(Sentence):
    rhs: Result


@dataclass
class FuncBody:
    sentencies: list[Sentence]


@dataclass
class SentenceBlock(Sentence):
    res: Result
    block: FuncBody


@dataclass
class Func:
    ident: str
    body: FuncBody


@dataclass
class Start:
    funcs: list[Func]


# TERMINALS
IDENT = pe.Terminal("IDENT", r"[A-Za-z][A-Za-z0-9_-]+", str)
INTEGER = pe.Terminal("INTEGER", r"[0-9]+", int)
VAR = pe.Terminal("VAR", r"(s|e|t)\.(([A-Za-z][A-Za-z0-9_-]*)|([0-9]+))", str)
STRING = pe.Terminal("STRING", r"'[^\n']*'|\"[^\n\"]*\"", func=str)

# NON TERMINALS
start = pe.NonTerminal("Start")
funcs = pe.NonTerminal("FuncDefs")
func = pe.NonTerminal("Func")
func_body = pe.NonTerminal("FuncBody")

sentencies = pe.NonTerminal("Sentencies")
sentence = pe.NonTerminal("Sentence")
lsh = pe.NonTerminal("Lhs")
condition = pe.NonTerminal("Condition")
pattern = pe.NonTerminal("Pattern")
pattern_parts = pe.NonTerminal("PatternParts")
pattern_part = pe.NonTerminal("PatternPart")
result = pe.NonTerminal("Result")
result_parts = pe.NonTerminal("ResultParts")
result_part = pe.NonTerminal("ResultPart")


start |= func, start, lambda f, s: Start([f] + s.funcs)
start |= lambda: Start([])

func |= (
    IDENT,
    "{",
    func_body,
    "}",
    lambda ident, body: Func(ident=ident, body=body),
)
func_body |= sentence, func_body, lambda s, body: FuncBody([s] + body.sentencies)
func_body |= lambda: FuncBody([])

sentence |= lsh, "=", result, ";", lambda left, right: SentenceResult(left, right)
sentence |= (
    lsh,
    ",",
    result,
    ":",
    "{",
    func_body,
    "}",
    ";",
    lambda left, res, body: SentenceBlock(left, res, body),
)

lsh |= pattern, lambda val: Lhs(val, [])
lsh |= (
    lsh,
    ",",
    condition,
    lambda left, cond: Lhs(left.pattern, [cond] + left.conditions),
)

condition |= result, ":", pattern, Condition

pattern |= pattern_part, pattern, lambda part, pat: Pattern([part] + pat.parts)
pattern |= lambda: Pattern([])
pattern_part |= "(", pattern, ")", PatternPartNested
pattern_part |= IDENT, PatternPartSimple
pattern_part |= STRING, PatternPartSimple
pattern_part |= INTEGER, PatternPartSimple
pattern_part |= VAR, PatternPartSimple

result |= (result_part, result, lambda part, res: Result([part] + res.parts))
result |= lambda: Result([])
result_part |= "<", IDENT, result, ">", ResultPartFunc
result_part |= IDENT, ResultPartSimple
result_part |= STRING, ResultPartSimple
result_part |= INTEGER, ResultPartSimple
result_part |= VAR, ResultPartSimple
result_part |= "(", result, ")", ResultPartNested


parser = pe.Parser(start)
assert parser.is_lalr_one()

parser.add_skipped_domain("\\s")
parser.add_skipped_domain("(/\\*[\\s\\S]*?\\*/)")


for filename in sys.argv[1:]:
    try:
        with open(filename) as f:
            tree = parser.parse(f.read())
            pprint(tree)
    except pe.Error as e:
        print(f"Ошибка {e.pos}: {e.message}")
    except Exception as e:
        print("Ошибка:", e)
