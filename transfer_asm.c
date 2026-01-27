#include "transfer_asm.h"
#include "AST.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int global_count;

int trans_asm(Ast* root)
{
    FILE* file = fopen("file_asm.txt", "w");

    int count = 0;
    
    run_tree(root, file, count);

    fclose(file);

    return 0;
}

int run_tree(Ast* root, FILE* file, int count)
{
    if(root -> left != NULL)
    {
        if(root -> type == NODE_WHILE)
        {
            count = global_count * 4;
            global_count++;
            fprintf(file, ": %d\n", count);
        }
        if(root -> type == NODE_IF)
        {
            count = global_count * 4;
            global_count++;
        }

        run_tree(root -> left, file, count);
    }
    if(root -> right != NULL)
    {
        run_tree(root -> right, file, count);
    }
    if(root -> else_body != NULL && root -> type == NODE_IF)
    {
        fprintf(file, "JMP %d\n", count + 3);
        fprintf(file, ": %d\n", count + 2);

        run_tree(root -> else_body, file, count);

        fprintf(file, ": %d\n", count + 3);
    }

    transfer_rules(root, file, count);

    return 0;
}

int transfer_rules(Ast* root, FILE* file, int count)
{
    switch(root -> type)
    {
        case END_OP:
        {
            break;
        }
        case END_PROG:
        {
            fprintf(file, "HLT");// прерывание программы
            break;
        }
        case VARIABLE_DEFINITION:
        {
            fprintf(file, "MOV %s\n", root -> var);
            break;
        }
        case VARIBLE:
        {
            fprintf(file, "PUSHV %s\n", root -> var); // когда прравниваем можно это не выводить слева хотя это не доолжно ни на что влиять
            break;
        }
        case CONS:
        {
            fprintf(file, "PUSH %s\n", root -> string);
            break;
        }
        case PRINT:
        {
            fprintf(file, "OUT\n");
            break;
        }
        case EQUAL:
        {
            fprintf(file, "POPV %s\n", root -> left -> var); // если все пойдет по пизде делает char* одним и тем же
            break;
        }
        case BINARY_OPTION:
        {
            switch (root -> type_node_bin)
            {
                case ADD:
                {
                    fprintf(file, "ADD\n");
                    break;
                }
                case SUB:
                {
                    fprintf(file, "SUB\n");
                    break;
                }
                case MUL:
                {
                    fprintf(file, "MUL\n");
                    break;
                }
                case DIV:
                {
                    fprintf(file, "DIV\n");
                    break;
                }
                default:
                    break;
            }

            break;
        }
        case LOGIC_OPTION:
        {
            switch (root -> type_node_log)
            {
                case MORE:
                {
                    fprintf(file, "JG %d\n", count + 1);
                    break;
                }
                case LESS:
                {
                    fprintf(file, "JL %d\n", count + 1);
                    break;
                }
                case SAME: 
                {
                    fprintf(file, "JE %d\n", count + 1);
                    break;
                }
                default:
                    break;
            }

            fprintf(file, "JMP %d\n", count + 2);
            fprintf(file, ": %d\n", count + 1);

            break;
        }
        case NODE_WHILE:
        {
            fprintf(file, "JMP %d\n", count);

            fprintf(file, ": %d\n", count + 2);
            break;
        }
        case NODE_IF:
        {
            break;
        }
        default:
        {
            printf("ошибка, несуществующий тип узла\n");
            //exit(1);
            break; 
        }
    }

    return 0;
}