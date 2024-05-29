import traceback
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


class SemanticError(pe.Error):
    pass


class RepeatedFuncDef(SemanticError):
    def __init__(self, pos, ident):
        self.pos = pos
        self.ident = ident

    @property
    def message(self):
        return f'Повторное определение функции {self.ident}'


class UnknownVar(SemanticError):
    def __init__(self, pos, varname):
        self.pos = pos
        self.varname = varname

    @property
    def message(self):
        return f'Необъявленная переменная {self.varname}'


class UnknownFunction(SemanticError):
    def __init__(self, pos, ident):
        self.pos = pos
        self.ident = ident

    @property
    def message(self):
        return f'Используемая функция ранее не была объявлена, а также не является встроенной: {self.ident}'

class Statement(abc.ABC):
    @abc.abstractmethod
    def check(self, idents, vars):
        pass

@dataclass
class ResultPart(abc.ABC):
    pass

@dataclass
class Result:
    parts: list[ResultPart]
    @pe.ExAction
    def create(attrs, coords, res_coord):
        part, result = attrs
        return Result([part] + result.parts)
    def check(self, idents, vars=set()):
        for part in self.parts:
            part.check(idents, vars)



@dataclass
class ResultPartNested:
    result: Result
    @pe.ExAction
    def create(attrs, coords, res_coord):
        result, = attrs
        return ResultPartNested(result)
    def check(self, idents, vars):
        self.result.check(idents, vars)

@dataclass
class PatternPart(abc.ABC):
    pass

@dataclass
class ResultPartFunc(PatternPart):
    ident: str
    ident_coord: pe.Position
    result: Result
    @pe.ExAction
    def create(attrs, coords, res_coord):
        ident, result = attrs
        cident = coords[0]
        return ResultPartFunc(ident, cident, result)
    def check(self, idents, vars):
        if self.ident not in idents:
            raise UnknownFunction(self.ident, self.ident_coord)
        self.result.check(idents, vars)
        

@dataclass
class Pattern:
    parts: list[PatternPart]
    @pe.ExAction
    def create(attrs, coords, res_coord):
        part, pattern = attrs
        return Pattern([part] + pattern.parts)
    def check(self, idents, vars=set()):
        pass
    def get_vars(self):
        result = set()
        for part in self.parts:
            if isinstance(part, Var):
                result.add(part.name())
        return result
    def get_vars_dict(self):
        result = {}
        for part in self.parts:
            if isinstance(part, Var):
                result[part.idx] = part.var_type
            # check nested vars 
        return result
    def is_l_pattern(self, vars={}):
        idx_to_type = vars
        e_type_counter = 0
        for part in self.parts:
            if isinstance(part, Var):
                if part.var_type == 'e':
                    e_type_counter += 1
                if e_type_counter > 1:
                    return False
                if part.var_type == 't':
                    if  (part.idx in idx_to_type) and (idx_to_type[part.idx] == 't'):
                        return False
                    idx_to_type[part.idx] = 't'
                if part.var_type == 'e':
                    if  (part.idx in idx_to_type) and (idx_to_type[part.idx] == 'e'):
                        return False
                    idx_to_type[part.idx] = 'e'
                    
            elif isinstance(part, PatternPartNested):
                res = part.is_l_pattern(vars)
                if not res:
                    return res
        
        return True
                
        

@dataclass
class PatternPartNested(PatternPart):
    pattern: Pattern
    @pe.ExAction
    def create(attrs, coords, res_coord):
        pattern, = attrs
        return PatternPartNested(pattern)
    def check(self, idents, vars=set()):
        pass
    def is_l_pattern(self, vars={}):
        return self.pattern.is_l_pattern(vars)



@dataclass
class Ident(ResultPart, PatternPart):
    name: str
    name_coord : pe.Position
    @pe.ExAction
    def create(attrs, coords, res_coord):
        name, = attrs
        cname,  = coords
        return Ident(name, cname)
    def check(self, idents, vars=set()):
        pass


@dataclass
class Var(ResultPart, PatternPart):
    var_coord : pe.Position
    idx: str
    var_type: str
    @pe.ExAction
    def create(attrs, coords, res_coord):
        name, = attrs
        cname,  = coords
        return Var(cname, name.split('.')[1], name[0])
    def check(self, idents, vars):
        if (self.var_type + '.' + self.idx) not in vars:
            raise UnknownVar(self.name(), self.var_coord)
    def name(self):
        return self.var_type + '.' + self.idx 
    
@dataclass
class Int(ResultPart, PatternPart):
    value: int
    value_coord : pe.Position
    @pe.ExAction
    def create(attrs, coords, res_coord):
        value, = attrs
        cvalue,  = coords
        return Int(value, cvalue)
    def check(self, idents, vars=set()):
        pass
 
@dataclass
class String(ResultPart, PatternPart):
    value: str
    value_coord : pe.Position
    @pe.ExAction
    def create(attrs, coords, res_coord):
        value, = attrs
        cvalue,  = coords
        return Int(value, cvalue)
    def check(self, idents):
        pass

@dataclass
class Condition:
    result: Result
    pattern: Pattern
    @pe.ExAction
    def create(attrs, coords, res_coord):
        result, pattern = attrs
        return Condition(result, pattern)
    # возвращать переменные паттернов условий
    def check(self, idents, pattern_vars=set()):
        vars = self.pattern.get_vars() | pattern_vars
        self.result.check(idents, vars)

@dataclass
class Lhs:
    pattern: Pattern
    conditions: list[Condition]
    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs_other, condition = attrs
        return Lhs(lhs_other.pattern, [condition] + lhs_other.conditions)
    def check(self, idents, vars=set()):
        new_vars = vars | self.pattern.get_vars()
        for condition in self.conditions:
            condition.check(idents, vars)
    def contains_not_l_patterns(self):
        if not self.pattern.is_l_pattern():
            return True
        vars = self.pattern.get_vars_dict()
        for cond in self.conditions:
            if not cond.pattern.is_l_pattern(vars):
                return True
            vars |= cond.pattern.get_vars_dict()
            
        return False


@dataclass
class Sentence:
    lhs: Lhs
    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs, = attrs
        return Sentence(lhs)
    def check(self, idents, vars=set()):
        pass

@dataclass
class SentenceResult(Sentence):
    rhs: Result
    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs, res = attrs
        return SentenceResult(lhs, res)
    def check(self, idents, vars=set()):
        self.lhs.check(idents)
        vars = self.lhs.pattern.get_vars()
        for cond in self.lhs.conditions:
            vars |= cond.pattern.get_vars()
        self.rhs.check(idents, vars)
    def contains_not_l_patterns(self):
        return self.lhs.contains_not_l_patterns()
        


@dataclass
class FuncBody():
    sentencies: list[Sentence]
    @pe.ExAction
    def create(attrs, coords, res_coord):
        sentence, body = attrs
        return FuncBody([sentence] + body.sentencies)
    def check(self, idents, vars=set()):
        for sentence in self.sentencies:
            sentence.check(idents, vars)
    def contains_not_l_patterns(self):
        for sentence in self.sentencies:
            if sentence.contains_not_l_patterns():
                return True
        return False 
     


@dataclass
class SentenceBlock(Sentence):
    res: Result
    block: FuncBody
    @pe.ExAction
    def create(attrs, coords, res_coord):
        lhs, res, block = attrs
        return SentenceBlock(lhs, res, block)
    def check(self, idents, vars=set()):
        vars = lhs.get_vars()
        res.check(idents, vars)
        block.check(idents, vars)
    def contains_not_l_patterns(self):
        return self.lhs.contains_not_l_patterns()
        

@dataclass
class Func:
    ident: str
    ident_coord: pe.Position
    body: FuncBody
    @pe.ExAction
    def create(attrs, coords, res_coord):
        ident, body = attrs
        cident = coords[0]
        return Func(ident, cident, body)
    def check(self, idents):
        if self.ident in idents:
            raise RepeatedFuncDef(self.ident_coord, self.ident)
        idents.add(self.ident)
        self.body.check(idents);
    def contains_not_l_patterns(self):
        for sentence in self.body.sentencies:
            res = sentence.contains_not_l_patterns()
            if res:
                return res
        return False
            
@dataclass
class Start:
    funcs: list[Func]
    # statements: list[Statement]
    @pe.ExAction
    def create(attrs, coords, res_coord):
        func, start = attrs
        return Start([func] + start.funcs)
    def check(self):
        idents = set()
        for func in self.funcs:
            func.check(idents | set([
                # I/O
                'Card', 'Print', 'Prout', 'Open', 'Get', 'Put', 'Putout',
                # Math
                'Add', 'Sub', 'Mul', 'Div', 'Divmod', 'Mod', 'Compare', 'Trunc', 'Real', 'ReafFun',
                # Stack ops
                'Br', 'Dg', 'Cp', 'Rp', 'Dgall',
                # Strings and symbols
                'Type', 'Numb', 'Symb', 'Implode', 'Explode', 'Chr', 'Ord', 'First', 'Last', 'Lenw', 'Lower', 'Upper',
                # Sys funcs
                'Step', 'Time', 'Arg', 'Mu', 'Up', 'Dn',
                ]))
            idents.add(func.ident)
    def print_funcs(self):
        for func in self.funcs:
            if not func.contains_not_l_patterns():
                pprint(func.ident)


# TERMINALS
IDENT = pe.Terminal("IDENT", r"[A-Za-z][A-Za-z0-9_-]*", str)
INTEGER = pe.Terminal("INTEGER", r"[0-9]+", int)
VAR = pe.Terminal("VAR", r"(s|e|t)\.(([A-Za-z][A-Za-z0-9_-]*)|([0-9]+))", str)
STRING = pe.Terminal("STRING", r"'[^\n']*'|\"[^\n\"]*\"", func=str)

# NON TERMINALS
start = pe.NonTerminal("Start")
func = pe.NonTerminal("Func")
func_body = pe.NonTerminal("FuncBody")

sentencies = pe.NonTerminal("Sentencies")
sentence = pe.NonTerminal("Sentence")
lhs = pe.NonTerminal("Lhs")
condition = pe.NonTerminal("Condition")
pattern = pe.NonTerminal("Pattern")
pattern_parts = pe.NonTerminal("PatternParts")
pattern_part = pe.NonTerminal("PatternPart")
result = pe.NonTerminal("Result")
result_parts = pe.NonTerminal("ResultParts")
result_part = pe.NonTerminal("ResultPart")


start |= func, start, Start.create
start |= lambda: Start([])

func |= (
    IDENT,
    "{",
    func_body,
    "}",
     Func.create,
)
func_body |= sentence, func_body, FuncBody.create
func_body |= lambda: FuncBody([])

sentence |= lhs, "=", result, ";", SentenceResult.create
sentence |= (
    lhs,
    ",",
    result,
    ":",
    "{",
    func_body,
    "}",
    ";",
    SentenceBlock.create,
)

lhs |= pattern, lambda val: Lhs(val, [])
lhs |= (
    lhs,
    ",",
    condition,
    Lhs.create,
)

condition |= result, ":", pattern, Condition.create

pattern |= pattern_part, pattern, Pattern.create
pattern |= lambda: Pattern([])
pattern_part |= "(", pattern, ")", PatternPartNested.create
pattern_part |= IDENT, Ident.create
pattern_part |= STRING, String.create
pattern_part |= INTEGER, Int.create
pattern_part |= VAR, Var.create

result |= result_part, result, Result.create
result |= lambda: Result([])
result_part |= "<", IDENT, result, ">", ResultPartFunc.create
result_part |= IDENT, Ident.create
result_part |= STRING, String.create
result_part |= INTEGER, Int.create
result_part |= VAR, Var.create
result_part |= "(", result, ")", ResultPartNested.create


parser = pe.Parser(start)
assert parser.is_lalr_one()

parser.add_skipped_domain("\\s")
parser.add_skipped_domain("(/\\*[\\s\\S]*?\\*/)")


for filename in sys.argv[1:]:
    try:
        with open(filename) as f:
            tree = parser.parse(f.read())
            tree.check()
            print('Семантических ошибок не найдено')
            tree.print_funcs()
    except pe.Error as e:
        print(f"Ошибка {e.pos}: {e.message}")
    except Exception as e:
        print("Ошибка:", e)
        traceback.print_exc()

    # try:
    #     with open(filename) as f:
    #         tree = parser.parse(f.read())
    #         tree.check()
    # except Exception:
