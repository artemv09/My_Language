#ifndef AST_H
#define AST_H

#include "lexer.h"

#include <ctype.h>

typedef struct PARSER
{
    Lexer* lexer;
    Token* token_now;
    Token* token_prev;

} Parser;

typedef enum
{
    END_OP,
    END_PROG,

    VARIABLE_DEFINITION,
    VARIBLE,
    CONS,

    EQUAL,

    BINARY_OPTION,

    LOGIC_OPTION,

    NODE_WHILE,
    NODE_IF,
    PRINT,

    FUNC_CALL,
    FUNC_DEF,
    RETURN

} PARSER_TYPE;

typedef enum
{
    ERR_BIN,
    ADD,
    SUB,
    MUL,
    DIV
} BINAR;

typedef enum
{
    ERR_LOG,
    MORE_SAME,
    LESS_SAME,
    MORE,
    LESS,
    SAME
} LOGIC;

typedef struct Ast
{
    PARSER_TYPE  type;

    struct Ast* left;
    struct Ast* right;

    union
    {
        BINAR type_node_bin;
        LOGIC type_node_log;
    };

    struct Ast* else_body;   

    char** name_arg;

    int count_arg = 0;
    union //это все дерьмо убрать!!!
    {
        char* string;
        
        char* var;

        char* binar_oper;

        char* logic_oper;

        char* assignment_name;

        char* fun_while_oper;

        char* if_else_oper;

        char* end_op_name;

        char* end_prog_name;

        char* fun_name;
    };
    
} Ast;

Ast* creat_node(Ast* ast_tree1, Ast* ast_tree2, PARSER_TYPE type, char* str);

Ast* creat_node_if(Ast* ast_tree, Ast* ast_tree1, Ast* ast_tree2, char* str);

void free_ast(Ast* ast);
void free_token(Token* token);
void free_lexer(Lexer* lexer);
void free_parser(Parser* parser);

char* read_file_without_newlines();


#endif