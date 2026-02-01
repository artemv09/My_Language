#ifndef TRANS_ASM
#define TRANS_ASM

#include "AST.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int global_count;
extern int global_count_arr_func;

typedef struct 
{
    int flag_call;
    char* name_func;
} Func_help;


typedef struct 
{
    int flag_oper_node = 0;

} Flag;

int definition_for_call(char* name_func, Func_help* arr);

int trans_asm(Ast* root);

int run_tree(Ast* root, FILE* file, int count, Func_help* arr);

int transfer_rules(Ast* root, FILE* file, int count, Func_help* arr);

#endif