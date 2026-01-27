#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct LEXER
{
    char symbol;
    unsigned count_symbol;
    char* contest;
} Lexer;


Lexer* lexer_creat(char* contest);

Lexer* lexer_assignment(Lexer* lexer);

Token* next_lexer_token(Lexer* lexer);

void lexer_skip(Lexer* lexer);

Token* lexer_string(Lexer* lexer);

char* lexer_id(Lexer* lexer);

char* lexer_special_symbol(Lexer* lexer);

Token* expected_token(Lexer* lexer, TYPE_TOKEN expected_type);

#endif