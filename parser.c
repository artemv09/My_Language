#include "lexer.h"
#include "parser.h"
#include "AST.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


Parser* inizial_parser(Lexer* lexer)
{
    if (!lexer) return NULL;

    Parser* parser = (Parser*)calloc(1, sizeof(Parser));

    parser -> lexer = lexer;
    parser -> token_now = NULL;
    parser -> token_prev = NULL;

    new_parser(parser);

    return parser;
}

Ast* parser_eof(Parser* parser)
{
    Ast* ast_tree = parser_opend(parser);

    if(parser -> token_now && parser -> token_now -> type != TOKEN_EOF)
    {
        printf("программа не наткнулась на EOF\n");

        if (ast_tree) free_ast(ast_tree);

        return NULL;
    }

    return ast_tree;
}

Ast* parser_opend(Parser* parser)
{
    Ast* ast_tree = parser_while(parser);
    
    char* str = strdup("END_OP");
    
    new_parser(parser);

    if(parser -> token_now -> type == TOKEN_EOF)
    {
        char* str1 = strdup("END_PROG");

        Ast* ast_tree1 = creat_node(NULL, NULL, END_PROG, str1);

        return creat_node(ast_tree, ast_tree1, END_OP, str);
    }

    Lexer* lexer_copy = (Lexer*)calloc(1, sizeof(struct LEXER));

    memcpy(lexer_copy, parser -> lexer, sizeof(struct LEXER)); // исправить этот кусок выглядит очень хуево

    Token* token_copy = next_lexer_token(lexer_copy);

    if(token_copy -> type == TOKEN_EOF || parser -> token_now -> type == TOKEN_RPAREN)
    {
        return creat_node(ast_tree, NULL, END_OP, str);
    }

    Ast* ast_tree1 = parser_opend(parser);

    return creat_node(ast_tree, ast_tree1, END_OP, str);
}

Ast* parser_while(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "while") == 0)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LBRACE) 
        {
            printf("Ожидалось '(' после while\n");
            free(str);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree = parser_log(parser);

        if(!ast_tree) 
        {
            free(str);
            return NULL;
        }

        if(parser -> token_now -> type != TOKEN_RBRACE) 
        {
            printf("Ожидалось ')' после условия while\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LPAREN) 
        {
            printf("Ожидалось '{' после условия while\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree2 = parser_opend(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        if (parser -> token_now -> type != TOKEN_RPAREN) 
        {
            printf("Ожидалось '}' после тела while\n");

            free(str);
            free_ast(ast_tree);
            free_ast(ast_tree2);

            return NULL;
        }

        Ast* result = creat_node(ast_tree, ast_tree2, NODE_WHILE, str);

        return result;
    }
    else
    {
        return parser_if(parser);
    }
}

Ast* parser_if(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "if") == 0)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LBRACE) 
        {
            printf("Ожидалось '(' после if\n");
            free(str);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree = parser_log(parser);

        if(!ast_tree) 
        {
            free(str);
            return NULL;
        }

        if(parser -> token_now -> type != TOKEN_RBRACE) 
        {
            printf("Ожидалось ')' после условия условия if\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

       
        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LPAREN) 
        {
            printf("Ожидалось '{' после условия if\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree1 = parser_opend(parser);

        if (!ast_tree1) 
        {
            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        if(parser -> token_now -> type != TOKEN_RPAREN) 
        {
            printf("Ожидалось '}' после тела if\n");

            free(str);
            free_ast(ast_tree);
            free_ast(ast_tree1);

            return NULL;
        }

        new_parser(parser);

        if(strcmp(parser -> token_now -> value, "else") == 0) 
        {
            new_parser(parser);

            if(parser -> token_now -> type != TOKEN_LPAREN) 
            {
                printf("Ожидалось '{' после условия else\n");

                free(str);
                free_ast(ast_tree);

                return NULL;
            }

            new_parser(parser);

            Ast* ast_tree2 = parser_opend(parser);

            if(!ast_tree2) 
            {
                free(str);
                free_ast(ast_tree);

                return NULL;
            }

            if(parser -> token_now -> type != TOKEN_RPAREN) 
            {
                printf("Ожидалось '}' после тела else\n");

                free(str);
                free_ast(ast_tree);
                free_ast(ast_tree2);

                return NULL;
            }

            //new_parser(parser);
            Ast* result;
            result = creat_node_if(ast_tree, ast_tree1, ast_tree2, str);
            
            return result;
        }
        else
        {
            printf("отсутствует else после if");
            return NULL;
        }
    }
    else
    {
        return parser_log(parser);
    }
}

Ast* parser_log(Parser* parser)
{
    Ast* ast_tree = parser_bin(parser);

    LOGIC type_node_log = is_operator_log(parser -> token_now -> value);

    if(type_node_log != ERR_LOG)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);
            return NULL;
        }

        ast_tree = creat_node(ast_tree, ast_tree2, LOGIC_OPTION, str);

        ast_tree -> type_node_log = type_node_log;
    }

    return ast_tree;
}

Ast* parser_bin(Parser* parser)
{
    Ast* ast_tree = parser_equal(parser);

    BINAR type_node_bin = is_operator_bin(parser -> token_now -> value);

    if(type_node_bin != ERR_BIN) 
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if(!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);
            return NULL;
        }

        ast_tree = creat_node(ast_tree, ast_tree2, BINARY_OPTION, str);

        ast_tree -> type_node_bin = type_node_bin;
    }

    return ast_tree;
}

Ast* parser_equal(Parser* parser)
{
    Ast* ast_tree = parser_print(parser);

    if(!ast_tree) return NULL;

    if(parser -> token_now && parser -> token_now -> type == TOKEN_EQUALS)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree); 

            return NULL;
        }

        Ast* ast_tree3 = creat_node(ast_tree, ast_tree2, EQUAL, str);

        return ast_tree3;
    }

    return ast_tree;
}

Ast* parser_print(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "print") == 0)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree = parser_var(parser);

        return creat_node(ast_tree, NULL, PRINT, str);
    }
    else
    {
        return parser_dif_var(parser);
    }
}

Ast* parser_dif_var(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "var") == 0)
    {
        new_parser(parser);

        Ast* ast_tree = parser_var(parser);

        if(ast_tree) 
        {
            ast_tree -> type = VARIABLE_DEFINITION;
        }

        return ast_tree;
    }
    else
    {
        return parser_var(parser);
    }
}

Ast* parser_var(Parser* parser)
{
    if(has_no_digits_simple(parser -> token_now -> value) && parser -> token_now -> type != TOKEN_STRING)
    {
        Ast* ast_node = creat_node(NULL, NULL, VARIBLE, strdup(parser -> token_now -> value));

        new_parser(parser);

        return ast_node;
    }
    else
    {
        return parser_const(parser);
    }
}

Ast* parser_const(Parser* parser)
{
    if(parser -> token_now -> type == TOKEN_ID || parser -> token_now -> type == TOKEN_STRING)
    {
        char* token_value = strdup(parser -> token_now -> value);

        Ast* ast_node = creat_node(NULL, NULL, CONS, token_value);

        new_parser(parser);

        return ast_node;
    }
    else
    {
        printf("ошибка в синтаксисе спустились до низу и нашлось совпадений %s\n", parser -> token_now -> value);

        return NULL;
    }
}

Parser* new_parser(Parser* parser)
{
    if(parser -> token_prev) 
    {
        free_token(parser -> token_prev);

        parser -> token_prev = NULL;
    }

    parser -> token_prev = parser -> token_now;
    
    parser -> token_now = next_lexer_token(parser -> lexer);

    printf("TOKEN (%s)\n", parser -> token_now -> value);

    return parser;
}

bool has_no_digits_simple(char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]) || str[i] == '_')
        {
            return true;
        }
    }
    return false;
}

BINAR is_operator_bin(char* str) 
{   
    if(strcmp(str, "+") == 0)
        return ADD;

    else if(strcmp(str, "-") == 0)
        return SUB;

    else if(strcmp(str, "*") == 0)
        return MUL;

    else if(strcmp(str, "\\") == 0)
        return DIV;

    else
        return ERR_BIN;
}

LOGIC is_operator_log(char* str) 
{   
    if(strcmp(str, "less_same") == 0)
        return LESS_SAME;

    else if(strcmp(str, "more_same") == 0)
        return MORE_SAME;

    else if(strcmp(str, "more") == 0)
        return MORE;

    else if(strcmp(str, "less") == 0)
        return LESS;

    else if(strcmp(str, "same") == 0)
        return SAME;

    else
        return ERR_LOG;
}
    