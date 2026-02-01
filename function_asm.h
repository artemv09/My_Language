#ifndef FUNC_H
#define FUNC_H

#include "asm.h"

int main_function();

int function_executor(Element_arr_asm* arr_func_asm, Stack* stack_struct);

int comparing_func(Element_arr_asm* arr_func_asm, Stack* stack_struct);

int push(Stack* stack_struct, int value);

int pop(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int peek(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int add(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int sub(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int mul(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int div(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int jmp(Stack* stack_struct, Element_arr_asm* arr_func_asm);   

int call(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int ret(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int pushv(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int popv(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int je(Stack* stack_struct, Element_arr_asm* arr_func_asm); 

int jg(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int jl(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int in();

int out(Stack* stack_struct, Element_arr_asm* arr_func_asm);

int defining_index_var(char* var, Stack* stack_struct);

int  binary_code_file(Element_arr_asm* arr_func_asm);

int isEmpty(Stack* stack_struct);

int isFull(Stack* stack_struct);

#endif