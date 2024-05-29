```
PROG -> STMNT PROG .
PROG -> .
STMNT -> tokens left token_name right TOKENS .
TOKENS ->  comma left token_name right TOKENS .
TOKENS -> .
STMNT -> start left nonterminal_name right .
STMNT -> left STMNT_INNER .
STMNT_INNER -> COMMENT_INNER .
STMNT_INNER -> RULE_INNER .
RULE_INNER -> nonterminal_name right is BODY .
BODY ->  BODIES dot .
BODIES -> left BODY_INNER right BODIES .
BODIES -> .
BODY_INNER -> nonterminal_name .
BODY_INNER -> token_name .
COMMENT_INNER ->  exclamation_point minus minus comment_inner minus minus right .
```

| Symbol          | Nullable? | Endable? | First set                                   | Follow set                                |
| --------------- | --------- | -------- | ------------------------------------------- | ----------------------------------------- |
| _PROG_          | Nullable  | Endable  | **tokens**, **left**, **start**             | <u>$</u>                                  |
| _STMNT_         |           | Endable  | **tokens**, **left**, **start**             | **tokens**, **left**, **start**, <u>$</u> |
| _TOKENS_        | Nullable  | Endable  | **comma**                                   | **tokens**, **left**, **start**, <u>$</u> |
| _STMNT_INNER_   |           | Endable  | **nonterminal_name**, **exclamation_point** | **tokens**, **left**, **start**, <u>$</u> |
| _RULE_INNER_    |           | Endable  | **nonterminal_name**                        | **tokens**, **left**, **start**, <u>$</u> |
| _BODY_          |           | Endable  | **left**, **dot**                           | **tokens**, **left**, **start**, <u>$</u> |
| _BODY_TMP_      | Nullable  |          | **left**                                    | **dot**                                   |
| _BODY_INNER_    |           |          | **token_name**, **nonterminal_name**        | **right**                                 |
| _COMMENT_INNER_ |           | Endable  | **exclamation_point**                       | **tokens**, **left**, **start**, <u>$</u> |
