#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "function.h"

int main_function()
{
    Element_arr_asm* arr_func_asm = (Element_arr_asm*)calloc(1, sizeof(Element_arr_asm));

    Stack* stack_struct = (Stack*)calloc(1, sizeof(Stack));

    stack_struct -> stack = (int*)calloc(1, sizeof(int));
    
    arr_func_asm = creat_bin_file(arr_func_asm, stack_struct);

    binary_code_file(arr_func_asm);

    function_executor(arr_func_asm, stack_struct);

    return 0;
}

int function_executor(Element_arr_asm* arr_func_asm, Stack* stack_struct)
{
    while((arr_func_asm[stack_struct -> top].type != HLT))
    {
        comparing_func(arr_func_asm, stack_struct);

        stack_struct -> top++;
    }

    return 0;
}

int comparing_func(Element_arr_asm* arr_func_asm, Stack* stack_struct)
{
    switch(arr_func_asm[stack_struct -> top].type)
    {
        case(PUSH):
        {
            int value = arr_func_asm[stack_struct -> top].number;

            push(stack_struct, value);
            return PUSH;
        }
        case(POP):
        {
            pop(stack_struct, arr_func_asm);
            return POP;
        }
        case(ADD_asm):
        {
            add(stack_struct, arr_func_asm);
            return ADD_asm;
        }
        case(SUB_asm):
        {
            sub(stack_struct, arr_func_asm);
            return SUB_asm;
        }
        case(MUL_asm):
        {
            mul(stack_struct, arr_func_asm);
            return MUL_asm;
        }
        case(DIV_asm):
        {
            div(stack_struct, arr_func_asm);
            return DIV_asm;
        }
        case(JMP):
        {
            jmp(stack_struct, arr_func_asm);
            return JMP;
        }
        case(COLON):
        {
            return COLON;
        }
        case(CALL):
        {
            call(stack_struct, arr_func_asm);
            return CALL;
        }
        case(RET):
        {
            ret(stack_struct, arr_func_asm);
            return RET;
        }
        case(PUSHV):
        {
            pushv(stack_struct, arr_func_asm);
            return PUSHV;
        }
        case(POPV):
        {
            popv(stack_struct, arr_func_asm);
            return POPV;
        }
        case(JE):
        {
            je(stack_struct, arr_func_asm);
            return JE;
        }
        case(JL):
        {
            jl(stack_struct, arr_func_asm);
            return JL;
        }
        case(JG):
        {
            jg(stack_struct, arr_func_asm);
            return JG;
        }
        case(MOV):
        {
           return MOV;
        }
        case(OUT):
        {
            out(stack_struct, arr_func_asm);
            return OUT;
        }
        default:
        {
            printf("не существует такой функции gg\n"); 
            printf("[%d]", arr_func_asm[stack_struct -> top].type);
            return ERROR;
        }
    }
} 

int push(Stack* stack_struct, int value)
{
    if(isFull(stack_struct) == 1)
    {
        printf("стек переполнен\n");
        exit(1);
    }

    stack_struct -> stack[stack_struct -> top_stack] = value;

    stack_struct -> top_stack++;

    stack_struct -> stack = (int*)realloc(stack_struct -> stack, (stack_struct -> top_stack  + 1) * sizeof(int));

    return 0;

}

int pop(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    if(isEmpty(stack_struct) == 1)
    {
        printf("стек опустел\n");
        exit(1);
    }

    int copy_first_ell = stack_struct -> stack[stack_struct -> top_stack - 1];

    stack_struct -> top_stack--;

    stack_struct -> stack = (int*)realloc(stack_struct -> stack, (stack_struct -> top_stack  + 1) * sizeof(int));

    return copy_first_ell;
}

int peek(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int upper_element = stack_struct -> stack[stack_struct -> top_stack - 1];

    return upper_element;
}

int add(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    int add_stack = copy1 + copy2;
   
    push(stack_struct, add_stack);

    return add_stack;
}

int sub(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    int sub_stack = copy2 - copy1;
   
    push(stack_struct, sub_stack);

    return sub_stack;
}

int mul(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    int mul_stack = copy1 * copy2;
   
    push(stack_struct, mul_stack);

    return mul_stack;
}

int div(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    int div_stack = copy2 / copy1;
   
    push(stack_struct, div_stack);

    return div_stack;
}

int jmp(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    stack_struct -> top = stack_struct -> mark[arr_func_asm[stack_struct -> top].number];

    //int copy_metka = arr_func_asm[stack_struct -> top].number;

    return 0;
}   

int call(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    return 0;
}

int ret(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    return 0;
}

int pushv(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int count = defining_index_var(arr_func_asm[stack_struct -> top].variable, stack_struct);

    push(stack_struct, stack_struct -> list_values_var[count].meaning);

    return 0;
}

int popv(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int count = defining_index_var(arr_func_asm[stack_struct -> top].variable, stack_struct);

    stack_struct -> list_values_var[count].meaning = pop(stack_struct, arr_func_asm);

    return 0;
}

int je(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    if(copy1  == copy2)
    {
        int copy_metka = arr_func_asm[stack_struct -> top].number;

        stack_struct -> top = stack_struct -> mark[copy_metka];

        return 0;
    }
    
    return 0;
}   

int jg(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    if(copy1 <  copy2)
    {
        int copy_metka = arr_func_asm[stack_struct -> top].number;

        stack_struct -> top = stack_struct -> mark[copy_metka];

        return 0;
    }
    
    return 0;
}

int jl(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    int copy1 = pop(stack_struct, arr_func_asm);
    int copy2 = pop( stack_struct, arr_func_asm);

    if(copy1 >  copy2)
    {
        int copy_metka = arr_func_asm[stack_struct -> top].number;

        stack_struct -> top = stack_struct -> mark[copy_metka];

        return 0;
    }

    return 0;
}

int in()
{
    return 0;
}

int out(Stack* stack_struct, Element_arr_asm* arr_func_asm)
{
    printf("out == [%d]\n", peek(stack_struct, arr_func_asm));

    return 0;
}

int defining_index_var(char* var, Stack* stack_struct)
{
    int count = 0;

    while(strcmp(stack_struct -> list_values_var[count].variable, var) != 0)
    {
        count++;
    }
    
    return count;
}

int  binary_code_file(Element_arr_asm* arr_func_asm)
{
    FILE* file_bin = fopen("file_bin.txt", "w");

    int count = 0;

    while(arr_func_asm[count].type != HLT)
    {
        fprintf(file_bin, "%d\n", arr_func_asm[count].type);
        count++;
    }

   return 0;
}

int isEmpty(Stack* stack_struct)
{
    if (stack_struct -> top_stack == 0)
    {
        return 1;
    }
    else
        return 0;
}
int isFull(Stack* stack_struct)
{
    if (stack_struct -> top_stack == MAXSIZE)
    {
        return 1;
    }

    return 0;
}