#ifndef TRANS_ASM
#define TRANS_ASM

#include "AST.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int global_count;

typedef struct 
{
    int flag_oper_node = 0;

} Flag;

int trans_asm(Ast* root);

int run_tree(Ast* root, FILE* file, int count);

int transfer_rules(Ast* root, FILE* file, int count);

#endif