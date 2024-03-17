% Лабораторная работа № 1.1 Раскрутка самоприменимого компилятора
% 9 февраля 2024 г.
% Киселев Кирилл, ИУ9-61Б

# Цель работы

Целью данной работы является ознакомление с раскруткой
самоприменимых компиляторов на примере модельного компилятора.

# Индивидуальный вариант

Компилятор BeRo. Сделать так, чтобы символы .. и :
перестали быть взаимозаменяемыми.

# Реализация

Для автоматизации запуска команд
по компиляции используется [Taskfile](https://taskfile.dev/)

```yaml
---
version: "3"
tasks:
  build:
    cmds: [./btpc64 <./btpc64-2.pas > ./btpc64-2]
  update:
    deps: [build]
    cmds: [./btpc64-2 <./btpc64-3.pas > ./btpc64-3]
  test:
    deps: [update]
    cmds: [./btpc64-3 <./hello.pas > ./hello, ./hello]
  test-feature:
    deps: [update]
    cmds: [./btpc64-3 <./feature.pas > ./feature, ./feature]
```

Для того чтобы, убрать возможность
использовать .. вместо : нужно добавить новый
токен для .. и модифицировать код,
который обрабатывает вхождение ..

Добавление нового токена TokRange 
```
      SymHALT=48;
      SymFUNC=49;
      SymPROC=50;
      TokRange=51;
```

Изменение обработки ..

```
end else if CurrentChar='.' then begin
  ReadChar;
  if CurrentChar='.' then begin
   ReadChar;
   CurrentSymbol:=TokRange;
  end else begin
   CurrentSymbol:=TokPeriod
  end;
```

Обработка токена TokRange при выводе типа Массива

```
procedure ArrayType(var t..integer);
var x:integer;
begin
 Types[t].Kind:=KindARRAY;
 GetSymbol;
 Constant(Types[t].StartIndex,x);
 MustBe(TypeINT,x);
 Expect(TokRange);
 Constant(Types[t].EndIndex,x);
 MustBe(TypeINT,x);
 
```

Теперь .. может использоваться лишь для объявления
типа массива, а : для объявления параметров функции.


Различие между файлами `btpc64.pas` и `btpc64-2.pas`:

```diff
--- btpc64.pas	2024-02-07 16:49:30.195638766 +0300
+++ btpc64-2.pas	2024-02-09 14:57:12.460828027 +0300
@@ -167,7 +167,8 @@
       SymHALT=48;
       SymFUNC=49;
       SymPROC=50;
-
+      TokRange=51;
+      
       IdCONST=0;
       IdVAR=1;
       IdFIELD=2;
@@ -291,6 +292,10 @@
   TokColon:begin
    Write('":" expected');
   end;
+  TokRange:begin
+   Write('".." expected');
+  end;
+
   TokEql:begin
    Write('"=" expected');
   end;
@@ -685,7 +690,7 @@
   ReadChar;
   if CurrentChar='.' then begin
    ReadChar;
-   CurrentSymbol:=TokColon;
+   CurrentSymbol:=TokRange;
   end else begin
    CurrentSymbol:=TokPeriod
   end;
@@ -1665,7 +1670,7 @@
  GetSymbol;
  Constant(Types[t].StartIndex,x);
  MustBe(TypeINT,x);
- Expect(TokColon);
+ Expect(TokRange);
  Constant(Types[t].EndIndex,x);
  MustBe(TypeINT,x);
  if Types[t].StartIndex>Types[t].EndIndex then begin
```


Различие между файлами `btpc64-2.pas` и `btpc64-3.pas`:


```diff
--- btpc64-2.pas	2024-02-09 14:57:12.460828027 +0300
+++ btpc64-3.pas	2024-02-09 15:04:34.215441586 +0300
@@ -193,7 +193,7 @@
       FunEOF=6;
       FunEOFLN=7;
 
-type TAlfa=array[1:MaximalAlfa] of char;
+type TAlfa=array[1..MaximalAlfa] of char;
 
      TIdent=record
       Name:TAlfa;
@@ -227,7 +227,7 @@
     CurrentSymbol:integer;
     CurrentIdentifer:TAlfa;
     CurrentNumber:integer;
-    CurrentString:array[1:255] of char;
+    CurrentString:array[1..255] of char;
     CurrentStringLength:integer;
     FunctionDeclarationIndex:integer;
     Keywords:array[SymBEGIN..SymPROC] of TAlfa;
@@ -239,7 +239,7 @@
     TypePosition:integer;
     Identifiers:array[0..MaximalIdentifiers] of TIdent;
     Types:array[1..MaximalTypes] of TType;
-    Code:array[0:MaximalCodeSize] of integer;
+    Code:array[0..MaximalCodeSize] of integer;
     CodePosition:integer;
     StackPosition:integer;
 
@@ -1378,7 +1378,7 @@
 end;
 
 procedure Statement;
-var L:array[1:MaximalCases] of integer;
+var L:array[1..MaximalCases] of integer;
     m,n,i,j,t,x,r,OldStackPosition:integer;
 begin
  if CurrentSymbol=TokIdent then begin
@@ -1609,9 +1609,9 @@
  end;
 end;
 
-procedure Block(L..integer); forward;
+procedure Block(L:integer); forward;
 
-procedure Constant(var c,t..integer);
+procedure Constant(var c,t:integer);
 var i,s:integer;
 begin
  if (CurrentSymbol=tOKsTRc) and (CurrentStringLength=1) then begin
@@ -1663,7 +1663,7 @@
 
 procedure TypeDefinition(var t:integer); forward;
 
-procedure ArrayType(var t..integer);
+procedure ArrayType(var t:integer);
 var x:integer;
 begin
  Types[t].Kind:=KindARRAY;
@@ -1976,7 +1976,7 @@
 
 const OutputCodeDataMaximalSize=262144;
 
-var OutputCodeData:array[1:OutputCodeDataMaximalSize] of char;
+var OutputCodeData:array[1..OutputCodeDataMaximalSize] of char;
     OutputCodeDataSize:integer;
 
 procedure EmitChar(c:char);
@@ -1993,7 +1993,7 @@
  EmitChar(chr(B));
 end;
 
-procedure EmitInt16(i..integer);
+procedure EmitInt16(i:integer);
 begin
  if i>=0 then begin
   EmitByte(i mod 256);
@@ -2005,7 +2005,7 @@
  end;
 end;
 
-procedure EmitInt32(i..integer);
+procedure EmitInt32(i:integer);
 begin
  if i>=0 then begin
   EmitByte(i mod 256);
@@ -2034,7 +2034,7 @@
  end;
 end;
 
-procedure OutputCodePutInt32(o,i..integer);
+procedure OutputCodePutInt32(o,i:integer);
 begin
  if i>=0 then begin
   OutputCodeData[o]:=chr(i mod 256);
@@ -2483,7 +2483,7 @@
  LastOutputCodeValue:=locMovEAXDWordPtrFORStateDestValue;
 end;
 
-var JumpTable:array[1:MaximalCodeSize] of integer;
+var JumpTable:array[1..MaximalCodeSize] of integer;
 
 procedure AssembleAndLink;
 var
```

# Тестирование

Тестирование на файле `hello.pas`

```
$ go-task test
task: [build] ./btpc64 <./btpc64-2.pas > ./btpc64-2
task: [update] ./btpc64-2 <./btpc64-3.pas > ./btpc64-3
task: [test] ./btpc64-3 <./hello.pas > ./hello
task: [test] ./hello
Hello, student!
```

Код программы, в которой
используются измененные возможности языка.

```pascal
program PrintNumbers;

function AddNumbers(a, b..integer): integer;
begin
    AddNumbers := a + b;
end;


var
  i: Integer;
  A: array[1:5] of Integer;
  
begin
  for i := 1 to 5 do
  begin
    A[i] := i;
  end;

  for i := 1 to 5 do
  begin
    writeln(A[i]);
  end;
end.
```



Запуск теста

```
$ go-task test-feature
task: [build] ./btpc64 <./btpc64-2.pas > ./btpc64-2
task: [update] ./btpc64-2 <./btpc64-3.pas > ./btpc64-3
task: [test-feature] ./btpc64-3 <./feature.pas > ./feature
task: [test-feature] ./feature
task: Failed to run task "test-feature":
    fork/exec /home/t1d333/bmstu/compilers/lab1/feature: exec format error
```

# Вывод

В рамках данной лабораторной работы были получены навыки
модификации исходного кода компилятора языка Pascal
с целью изменения возможностей языка. Также были
получены навыки по раскрутке самоприменимого компилятора.
