% Лабораторная работа № 1.4 «Лексический распознаватель»
% <лабораторная ещё не сдана>
% Кирилл Киселёв, ИУ9-61Б

# Цель работы

‹переписываете цель работы из задания›

# Индивидуальный вариант

‹переписываете индивидуальный вариант›

# Реализация

Лексическая структура языка — регулярные выражения для доменов:

- …
- …

Граф недетерминированного распознавателя:

```dot
…
```

Граф детерминированного распознавателя:

```dot
digraph G {
  rankdir=LR
  start[shape=circle]
  1[label="IDENT", shape=doublecircle]
  2[label="IDENT", shape=doublecircle]
  3[label="IDENT", shape=doublecircle]
  4[label="SET", shape=doublecircle]
  5[label="IDENT", shape=doublecircle]
  6[label="IDENT", shape=doublecircle]
  7[label="UNSET", shape=doublecircle]
  8[label="IDENT", shape=doublecircle]
  9[label="L_PAREN", shape=circle]
  10[label="BR", shape=doublecircle]
  11[label="STR", shape=doublecircle]
  12[label="STR_INNER", shape=circle]
  13[label="NUM", shape=doublecircle]
  14[label="IDENT", shape=doublecircle]

  start -> 1 [label="u"]
  start -> 2 [label="s"]
  start -> 8 [label="a-rtv-zA-Z0-9"]
  start -> 13 [label="0-9"]
  start -> 9 [label="("]


  4 -> 8 [label="a-zA-Z0-9"]
  7 -> 8 [label="a-zA-Z0-9"]

  2 -> 3 [label="e"]
  3 -> 4 [label="t"]
  1 -> 14 [label="n"]
  14 -> 5 [label="s"]
  14 -> 8[label="a-rt-zA-Z0-9"]
  5 -> 6 [label="e"]
  6 -> 7 [label="t"]
  1 -> 8 [label="a-rt-zA-Z0-9"]
  2 -> 7 [label="a-df-zA-Z0-9"]
  3 -> 8 [label="a-su-zA-Z0-9"]
  5 -> 8 [label="a-df-zA-Z0-9"]
  6 -> 8 [label="a-su-zA-Z0-9"]

  8 -> 8 [label="a-zA-Z0-9"]



  9 -> 10 [label=")"]
  9 -> 12 [label="a-ZA-Z0-9 ' '"]

  12 -> 12 [label="a-ZA-Z0-9 ' '"]
  12 -> 11 [label=")"]

  13 -> 13 [label="0-9"]
}
```

Реализация распознавателя:

```java
…
```

# Тестирование

Входные данные

```
…
```

Вывод на `stdout`

```
…
```

# Вывод

‹пишете, чему научились›
