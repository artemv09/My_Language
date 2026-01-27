#include "transfer_asm.h"
#include "AST.h"
#include "asm.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Functions word_comparison(char* word)
{
    if(strcmp(word, "PUSH") == 0) return PUSH;
    
    else if(strcmp(word, "POP") == 0) return POP;
    
    else if(strcmp(word, "ADD") == 0) return ADD_asm;
    
    else if(strcmp(word, "SUB") == 0) return SUB_asm;
   
    else if(strcmp(word, "MUL") == 0) return MUL_asm;
    
    else if(strcmp(word, "DIV") == 0) return DIV_asm;
    
    else if(strcmp(word, "JMP") == 0) return JMP;
    
    else if(strcmp(word, "JE") == 0) return JE;
    
    else if(strcmp(word, "JG") == 0) return JG;
    
    else if(strcmp(word, "JL") == 0) return JL;
    
    else if(strcmp(word, "CALL") == 0) return CALL;
    
    else if(strcmp(word, ":") == 0) return COLON;

    else if(strcmp(word, "RET") == 0) return RET;
    
    else if(strcmp(word, "IN") == 0) return IN;

    else if(strcmp(word, "OUT") == 0) return OUT;

    else if(strcmp(word, "SQRT") == 0) return SQRT;

    else if(strcmp(word, "PUSHV") == 0) return PUSHV;

    else if(strcmp(word, "POPV") == 0) return POPV;

    else if(strcmp(word, "MOV") == 0) return MOV;
    
    else if(strcmp(word, "HLT") == 0) return HLT;
    
    else
    {
        printf("ошибка чтения функции :%s\n", word);
        return ERROR;
    }
}

Element_arr_asm* creat_bin_file(Element_arr_asm* arr_func_asm, Stack* stack_struct)
{
    FILE* file_asm = fopen("file_asm.txt", "r");

    if(!file_asm)
    {
        printf("ошибка открытия файла\n");
        return 0;
    }
    
    stack_struct -> list_values_var = (Variable_def*)calloc(1, sizeof(Variable_def));

    char str[MAXSIZE];
    int count = 0;

    fscanf(file_asm, "%s", str);

    while(strcmp(str, "HLT") != 0)
    {
        arr_func_asm[count].type = word_comparison(str);

        if(arr_func_asm[count].type == ERROR)
        {
            printf("%d\n", count);
            break;
        }

        fgetc(file_asm);

        if(arr_func_asm[count].type == COLON)
        {
            int index = 0;
            fscanf(file_asm, "%d", &index);

            stack_struct -> mark[index] = count; //хз может тут нужен +1
        }
        
        else if(arr_func_asm[count].type == PUSHV || arr_func_asm[count].type == POPV)
        {
            char str2[MAXSIZE];

            fscanf(file_asm, "%s", str2);

            arr_func_asm[count].variable = strdup(str2);
        }

        else if(arr_func_asm[count].type == MOV)
        {
            char str2[MAXSIZE];
            fscanf(file_asm, "%s", str2);

            stack_struct -> list_values_var[stack_struct -> count_var].variable = strdup(str2);

            stack_struct -> list_values_var[stack_struct -> count_var].meaning = 0;

            stack_struct -> count_var++;

            stack_struct -> list_values_var = (Variable_def*)realloc(stack_struct -> list_values_var, (stack_struct -> count_var + 1) * sizeof(Variable_def));
        }

        else if(arr_func_asm[count].type != ADD_asm && arr_func_asm [count].type != SUB_asm && arr_func_asm[count].type != MUL_asm && arr_func_asm[count].type != DIV_asm && arr_func_asm[count].type != OUT)
        {
            int numb = 0;

            fscanf(file_asm, "%d", &numb);

            arr_func_asm[count].number = numb;

            fgetc(file_asm);
        }

        count++;

        memset(str, 0, sizeof(str));

        arr_func_asm = (Element_arr_asm*)realloc(arr_func_asm, (count + 1) * sizeof(Element_arr_asm));

        fscanf(file_asm, "%s", str);
    }

    arr_func_asm[count].type = word_comparison(str);

    fclose(file_asm);

    printf("массив инструкций создан : file_bin.txt\n");

    return arr_func_asm;
}

