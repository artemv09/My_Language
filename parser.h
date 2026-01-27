#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AST.h"

#include <stdio.h>
#include <stdbool.h>

bool has_no_digits_simple(char* str);

Token* expected_token(Lexer* lexer, TYPE_TOKEN expected_type);

Ast* parser_print(Parser* parser);

Parser* inizial_parser(Lexer* lexer);

Ast* parser_eof(Parser* parser);

Ast* parser_opend(Parser* parser);

Ast* parser_while(Parser* parser);

Ast* parser_if(Parser* parser);

Ast* parser_bin(Parser* parser);

Ast* parser_log(Parser* parser);

Ast* parser_equal(Parser* parser);

Ast* parser_dif_var(Parser* parser);

Ast* parser_var(Parser* parser);

Ast* parser_const(Parser* parser);

Parser* new_parser(Parser* parser);

BINAR is_operator_bin(char* str);

LOGIC is_operator_log(char* str);

#endif