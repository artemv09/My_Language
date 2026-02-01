#ifndef ASM_H
#define ASM_H

#define MAXSIZE 1000

typedef enum
{
    PUSH = 0,
    POP = 1,    
    ADD_asm = 2,
    SUB_asm = 3,
    MUL_asm = 4,
    DIV_asm = 5,
    JMP = 6,
    JE = 7,
    JG = 8,
    JL = 9,
    COLON = 10,
    CALL = 11,
    RET = 12,
    HLT = 13,
    IN = 14,
    OUT = 15,
    SQRT = 16,
    ERROR = 17,
    MOV = 18,
    PUSHV = 19,
    POPV = 20

} Functions;

typedef struct
{
    char* variable;
    int meaning;

} Variable_def;

typedef struct 
{
    Functions type;

    union 
    {
        int number;
        char* variable;
    };

} Element_arr_asm;

typedef struct 
{
    int top = 0; // передвигаем по массиву исполняемых функций
    int top_stack = 0;

    int count_var = 0;

    int* stack;

    int transition_ret = 0;
    
    int mark[MAXSIZE];

    Variable_def* list_values_var;

} Stack;

Functions word_comparison(char* word);

Element_arr_asm* creat_bin_file(Element_arr_asm* arr, Stack* stack_struct);

#endif