#pragma once

#include "symbol.h"
#define TERMINALS_COUNT 13
#define NONTERMINALS_COUNT 9
#define MAX_CELL_SIZE 256

constexpr Symbol TABLE[NONTERMINALS_COUNT][TERMINALS_COUNT][MAX_CELL_SIZE] = {

    /* PROG */

    {
        /*   END               */ {EMPTY},
        /*   TOKENS_KW         */ {STMNT, DOT, PROG, EMPTY},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {STMNT, DOT, PROG, EMPTY},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {STMNT, DOT, PROG, EMPTY},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    /* STMNT */

    {
        /*   END               */ {ERROR},
        /*   TOKENS_KW         */
        {TOKENS_KW, O_BRACKET, TOKEN_NAME, C_BRACKET, TOKENS, EMPTY},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {O_BRACKET, STMNT_INNER, EMPTY},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */
        {START_KW, O_BRACKET, NONTERMINAL_NAME, C_BRACKET, EMPTY},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // TOKENS,

    {
        /*   END               */ {EMPTY},
        /*   TOKENS_KW         */ {EMPTY},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {EMPTY},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */
        {COMMA, O_BRACKET, TOKEN_NAME, C_BRACKET, TOKENS, EMPTY},
        /*   START_KW          */ {EMPTY},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {EMPTY},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // STMNT_INNER,

    {
        /*   END               */ {EMPTY},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {ERROR},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */ {RULE_INNER, EMPTY},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */ {COMMENT_INNER, EMPTY},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // RULE_INNER,

    {

        /*   END               */ {ERROR},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {ERROR},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */
        {NONTERMINAL_NAME, C_BRACKET, IS, BODY, EMPTY},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // BODY,

    {
        /*   END               */ {ERROR},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {BODIES, EMPTY},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {BODIES, EMPTY},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // BODY_TMP,

    {
        /*   END               */ {ERROR},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */
        {O_BRACKET, BODY_INNER, C_BRACKET, BODIES, EMPTY},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {EMPTY},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // BODY_INNER,

    {
        /*   END               */ {ERROR},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {TOKEN_NAME, EMPTY},
        /*   O_BRACKET         */ {ERROR},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */ {NONTERMINAL_NAME, EMPTY},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */ {ERROR},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

    // COMMENT_INNER,

    {
        /*   END               */ {ERROR},
        /*   TOKENS_KW         */ {ERROR},
        /*   TOKEN_NAME        */ {ERROR},
        /*   O_BRACKET         */ {ERROR},
        /*   C_BRACKET         */ {ERROR},
        /*   COMMA             */ {ERROR},
        /*   START_KW          */ {ERROR},
        /*   NONTERMINAL_NAME  */ {ERROR},
        /*   IS                */ {ERROR},
        /*   DOT               */ {ERROR},
        /*   EXCLAMATION_POINT */
        {EXCLAMATION_POINT, MINUS, MINUS, COMMENT_BODY, MINUS, MINUS, C_BRACKET,
         EMPTY},
        /*   MINUS             */ {ERROR},
        /*   COMMENT_BODY      */ {ERROR},
    },

};
