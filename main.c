#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "graphviz.h"
#include "transfer_asm.h"
#include "asm.h"
#include "function.h"

int main()
{
    char* arr = read_file_without_newlines();

    printf("Начало парсинга строки: %s\n", arr);

    Lexer* lexer = lexer_creat(arr);

    if(!lexer) 
    {
        printf("Ошибка создания лексера\n");
        return 1;
    }

    Parser* parser = inizial_parser(lexer);

    if(!parser) 
    {
        printf("Ошибка создания парсера\n");
        free_lexer(lexer);
        return 1;
    }

    Ast* ast = parser_eof(parser);
  

    printf("AST успешно создан\n");

    graphviz(ast);

    trans_asm(ast);

    main_function();
    
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);

    printf("конец\n");

    return 0;
}
