#include "AST.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Ast* creat_node(Ast* ast_tree1, Ast* ast_tree2, PARSER_TYPE type, char* str)
{
    Ast* ast_node = (Ast*)calloc(1, sizeof(Ast));

    ast_node -> type = type;

    ast_node -> left = ast_tree1;
    ast_node -> right = ast_tree2;

    switch (type)
    {
        case EQUAL:
        {
            ast_node -> assignment_name = str;
            break;
        }
        case VARIBLE:
        {
            ast_node -> var = str;
            break;
        }
        case CONS:
        {
            ast_node -> string = str;
            break;
        }
        case BINARY_OPTION:
        {
            ast_node -> binar_oper = str;
            break;
        }
        case LOGIC_OPTION:
        {
            ast_node -> logic_oper = str;
            break;
        }
        case VARIABLE_DEFINITION:
        {
            ast_node -> var = str;
            break;
        }
        case NODE_WHILE:
        {
            ast_node -> fun_while_oper = str;
            break;
        }
        case END_OP:
        {
            ast_node -> end_op_name = str;
            break;
        }
        case END_PROG:
        {
            ast_node -> end_prog_name = str;
            break;
        }
        case PRINT:
        {
            ast_node -> var = str;
            break;
        }

        default:
            printf("ошибка такого типа не было заявлено: %d\n", type);
            free(str);
            free(ast_node);
            return NULL;
    }

    return ast_node;
}

Ast* creat_node_if(Ast* ast_tree, Ast* ast_tree1, Ast* ast_tree2, char* str)
{
    Ast* ast_node = (Ast*)calloc(1, sizeof(Ast));

    ast_node -> type = NODE_IF;

    ast_node -> if_else_oper = str;

    ast_node -> left = ast_tree;
    ast_node -> right = ast_tree1;
    ast_node -> else_body = ast_tree2;

    return ast_node;
}

void free_ast(Ast* ast)
{
    if(!ast) return;

    free_ast(ast -> left);
    free_ast(ast -> right);

    switch (ast -> type)
    {
        case EQUAL:
        {
            if(ast -> assignment_name) free(ast -> assignment_name);
            break;
        }
        case VARIBLE:
        {
            if(ast -> var) free(ast -> var);
            break;
        }
        case CONS:
        {
            if(ast -> string) free(ast -> string);
            break;
        }
        case BINARY_OPTION:
        {
            if(ast -> binar_oper) free(ast -> binar_oper);
            break;
        }
        case LOGIC_OPTION:
        {
            if(ast -> logic_oper) free(ast -> logic_oper);
            break;
        }
        case VARIABLE_DEFINITION:
        {
            if(ast -> var) free(ast -> var);
            break;
        }
        case NODE_WHILE:
        {
            if(ast -> fun_while_oper) free(ast -> fun_while_oper);
            break;
        }
        case END_OP:
        {
            if(ast -> end_op_name) free(ast -> end_op_name);;
            break;
        }

        default:
            break;
    }

    free(ast);
}

void free_token(Token* token)
{
    if(!token) return; 

    if(token -> value) 
    {
        free(token -> value);
        token -> value = NULL;
    }

    free(token);
}

void free_lexer(Lexer* lexer)
{
    if(lexer) 
    {
        free(lexer);
    }
}

void free_parser(Parser* parser)
{
    if(parser) 
    {
        free(parser);
    }
}

char* read_file_without_newlines()
{
    FILE* file = fopen("text.txt", "r");
    
    if(!file) 
    {
        perror("Ошибка открытия файла");
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) 
    {
        fclose(file);
        return strdup("");  
    }


    char* str = (char*)malloc(file_size + 1);

    if (!str) 
    {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    int index = 0;
    int symbol;

    while ((symbol = fgetc(file)) != EOF) 
    {
        if (symbol == '\n') 
        {
            str[index++] = ' ';
        } 
        else 
        {
            str[index++] = (char)symbol;
        }
    }
    
    str[index] = '\0';  

    fclose(file);

    return str;
}