#include "lexer.h"
#include "AST.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* lexer_creat(char* contest)
{
    if(!contest) return NULL;

    Lexer* lexer = (Lexer*)calloc(1, sizeof(Lexer));

    if(!lexer) return NULL;

    lexer -> count_symbol = 0;
    lexer -> contest = contest;
    lexer -> symbol = *contest;

    return lexer;
}

Lexer* lexer_assignment(Lexer* lexer)
{
    if (!lexer) return NULL;

    lexer -> count_symbol++;

    lexer -> symbol = lexer -> contest[lexer -> count_symbol];

    return lexer;
}

Token* next_lexer_token(Lexer* lexer)
{
    if(!lexer) return NULL;

    while(lexer -> symbol != '\0')
    {
        if(lexer -> symbol == ' ' || lexer -> symbol == '\n')
        {
            lexer_skip(lexer);
            
            return next_lexer_token(lexer); //изменение вместо countinio
        }

        if(lexer -> symbol == '"')
        {
            return lexer_string(lexer);
        }

        if(isalnum(lexer -> symbol))
        {
            return create_token(TOKEN_ID, lexer_id(lexer));
        }

        //если надо остальные спецсимволы прописывать здесь
        switch(lexer -> symbol)
        {
            case '=':
                return create_token(TOKEN_EQUALS, lexer_special_symbol(lexer));

            case ',':
                return create_token(TOKEN_COMMA, lexer_special_symbol(lexer));

            case ';':
                return create_token(TOKEN_SEMICOLON, lexer_special_symbol(lexer));

            case '{':
                return create_token(TOKEN_LPAREN, lexer_special_symbol(lexer));

            case '}':
                return create_token(TOKEN_RPAREN, lexer_special_symbol(lexer));

            case '(':
                return create_token(TOKEN_LBRACE, lexer_special_symbol(lexer));

            case ')':
                return create_token(TOKEN_RBRACE, lexer_special_symbol(lexer));

            default:
            {
                return create_token(TOKEN_ID, lexer_special_symbol(lexer));
            }
        }
    }

    return create_token(TOKEN_EOF, "\0");
}

void lexer_skip(Lexer* lexer)
{
    if(!lexer) return;

    while(lexer->symbol == ' ' || lexer -> symbol == '\n')
    {
        lexer_assignment(lexer);
    }
}

Token* lexer_string(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(1, sizeof(char));

    if(!value) return NULL;

    int count = 0;

    lexer_assignment(lexer); // Пропускаем первую кавычку

    while (lexer -> symbol != '"' && lexer -> symbol != '\0')
    {
        char* temp = (char*)realloc(value, (count + 2) * sizeof(char));

        if(!temp)
        {
            free(value);
            return NULL;
        }

        value = temp;

        value[count] = lexer -> symbol;

        lexer_assignment(lexer);
        count++;
    }

    value[count] = '\0';

    if(lexer->symbol == '"') 
    {
        lexer_assignment(lexer); // Пропускаем закрывающую кавычку
    }

    return create_token(TOKEN_STRING, value);
}

char* lexer_id(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(1, sizeof(char));

    int count = 0;

    while (isalnum(lexer -> symbol))
    {
        char* temp = (char*)realloc(value, (count + 2) * sizeof(char));

        value = temp;

        value[count] = lexer -> symbol;

        lexer_assignment(lexer);
        count++;
    }

    value[count] = '\0';

    return value;
}

char* lexer_special_symbol(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(2, sizeof(char));

    value[0] = lexer -> symbol;

    value[1] = '\0';

    lexer_assignment(lexer);

    return value;
}

Token* expected_token(Lexer* lexer, TYPE_TOKEN expected_type)
{
    Token* next_token = next_lexer_token(lexer);

    if(next_token && next_token -> type == expected_type)
    {
        return next_token;
    }
    else
    {
        printf("ошибка в логике следующий токен не совпадает с ожидаемым\n");

        if (next_token) 
        {
            free_token(next_token);
        }
        exit(1);
    }
}