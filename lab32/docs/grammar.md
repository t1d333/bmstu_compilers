<!-- Start ::= Func Start | . -->
<!-- Func ::= ident '{' FuncBody '}' . -->
<!-- FuncBody ::= Pattern BodyPartRhs FuncBody | . -->
<!-- BodyPartRhs ::= ',' Result ':' BodyPartEnd | '=' Result ';' . -->
<!-- BodyPartEnd ::= Pattern BodyPartRhs | '{' FuncBody '}' ';' . -->
<!-- Result ::= (ResultPart)* -->
<!-- ResultPart ::= ident | string | int | var | '(' Result ')' | '<' ident Result '>' . -->
<!-- Pattern ::= (ResultPart)* -->
<!-- PatternPart ::= ident | string | int | var | '(' Pattern ')' . -->
<!-- string ::= \'[^\n]*\' -->
<!-- int ::= [0-9]+ -->
<!-- ident ::= [A-Za-z][A-Za-z0-9_-]+ -->
<!-- var ::= (s|e|t)\.(([A-Za-z][A-Za-z0-9_-]*)|([0-9]+)) -->


Start -> Func Start | .
# Func -> IDENT { FuncBody }
# FuncBody -> Sentence FuncBody | .
# Sentence -> Lhs = Result; | Lhs, Result : { FuncBody };
# Lhs -> Pattern | Lhs, Condition
# Condition -> Result : Pattern
# Result -> ResultPart Result | eps
# ResultPart -> INDENT | 'STRING' | INTEGER | VAR | (Result) | < IDENT Result >
# Pattern -> PatternPart Pattern | eps
# PatternPart ->  INDENT | 'STRING' | INTEGER | VAR | (Pattern)

























Start -> Func Start | .
Func -> ident co FuncBody cc .
FuncBody -> Lhs SentenceRhs FuncBody | .
Lhs -> PatternPart Pattern Conditions | .
SentenceRhs -> eq Result semicolon | comma Result colon co FuncBody cc semicolon .
Conditions -> comma Result colon Pattern Conditions | .
Result -> ResultPart Result | .
ResultPart -> ident | q string q | int | var | o Result c | ao ident Result ac .
Pattern -> PatternPart Pattern | .
PatternPart -> ident | q string q | int | var | o Pattern c .

Start -> Func Start | EOF .
Func -> ident co FuncBody cc .
FuncBody -> BodyPart FuncBody | .
BodyPart -> PatternPart Pattern BodyPartRhs .
BodyPartRhs -> comma Result colon BodyPartEnd | eq Result semicolon .
BodyPartEnd -> Pattern BodyPartRhs | co FuncBody cc semicolon .
Result -> ResultPart Result | .
ResultPart -> ident | q string q | int | var | o Result c | ao ident Result ac .
Pattern -> PatternPart Pattern | .
PatternPart -> ident | q string q | int | var | o Pattern c .


