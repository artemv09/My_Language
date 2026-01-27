#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef enum
{
    END_OP,
    END_PROG,

    VARIABLE_DEFINITION,
    VARIBLE,
    CONS,

    FUNC_DEFINITION,
    FUNC_CALL,

    EQUAL,

    BINARY_OPTION,

    LOGIC_OPTION,

    NODE_WHILE,
    NODE_IF

} PARSER_TYPE;

typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV
} BINAR;

typedef enum
{
    OR,
    AND,
    QUALL_QUALL,
    MORE,
    LESS
} LOGIC;

typedef struct Ast
{
    PARSER_TYPE  type;

    union
    {
        struct
        {
            char* string;
        
            struct Ast* node_left_cons;
            struct Ast* node_right_cons;
        } cons;

        struct
        {
            char* var;

            struct Ast* node_left_var;
            struct Ast* node_right_var;
        } varible;

        struct
        {
            char* binar_oper;

            struct Ast* node_left_bin;
            struct Ast* node_right_bin;
        } bin_op;

        struct
        {
            char* logic_oper;

            struct Ast* node_left_log;
            struct Ast* node_right_log;
        } log_op;

        //присваивание
        struct
        {
            char* assignment_name;
            struct Ast* left;
            struct Ast* right;
        } assignment;

        struct
        {
            char* fun_while_oper;
            struct Ast* condition;
            struct Ast* body;
        } fun_while;

        struct
        {
            char* if_else_oper;
            struct Ast* if_condition;
            struct Ast* if_body;
            struct Ast* else_body;

        } fun_if;

        struct
        {
            char* name;
            struct Ast* left;
            struct Ast* right;

        } declaring_variable;

        struct
        {
            char* end_op_name;
            struct Ast* left;
            struct Ast* right;
        } end_op;

        struct
        {
            char* end_prog_name;
            struct Ast* left;
            struct Ast* right;
        } end_prog;
        
        //добавить тела функций
    };

} Ast;

typedef enum
{
    TOKEN_ID, // индификатор пока что туда пихнем все
    TOKEN_STRING,
    TOKEN_EQUALS, // равно
    TOKEN_COMMA, // запятая
    TOKEN_SEMICOLON, //точка с зяпятой
    TOKEN_LPAREN, // {
    TOKEN_RPAREN,
    TOKEN_LBRACE, // (
    TOKEN_RBRACE,
    TOKEN_EOF

} TYPE_TOKEN;

typedef struct Token
{
    TYPE_TOKEN type;
    char* value;

} Token;

typedef struct LEXER
{
    char symbol;
    unsigned count_symbol;
    char* contest;
} Lexer;

int graphviz(Ast* root);

int made_graphviz_dot(Ast* root, FILE* file);

Token* create_token(TYPE_TOKEN type, char* value);

Lexer* lexer_creat(char* contest);

Lexer* lexer_assignment(Lexer* lexer);

Token* next_lexer_token(Lexer* lexer);

void lexer_skip(Lexer* lexer);

Token* lexer_string(Lexer* lexer);

char* lexer_id(Lexer* lexer);

char* lexer_special_symbol(Lexer* lexer);


typedef struct PARSER
{
    Lexer* lexer;
    Token* token_now;
    Token* token_prev;
} Parser;

Token* expected_token(Lexer* lexer, TYPE_TOKEN expected_type);

Parser* inizial_parser(Lexer* lexer);

Ast* parser_eof(Parser* parser);

Ast* parser_opend(Parser* parser);

Ast* parser_while(Parser* parser);

Ast* parser_if(Parser* parser);

Ast* parser_log(Parser* parser);

Ast* parser_bin(Parser* parser);

Ast* parser_equal(Parser* parser);

Ast* parser_dif_var(Parser* parser);

Ast* parser_var(Parser* parser);

Ast* parser_const(Parser* parser);

Parser* new_parser(Parser* parser);

bool has_no_digits_simple(const char* str);

Ast* creat_node(Ast* ast_tree1, Ast* ast_tree2, PARSER_TYPE type, char* str);

Ast* creat_node_if(Ast* ast_tree, Ast* ast_tree1, Ast* ast_tree2, char* str);

Token* create_token(TYPE_TOKEN type, char* value);

int is_operator(char* str);

void free_ast(Ast* ast);
void free_token(Token* token);
void free_lexer(Lexer* lexer);
void free_parser(Parser* parser);

char* read_file_without_newlines();

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

    // Инициализируем первый токен

    Ast* ast = parser_eof(parser);
    
    if (ast) 
    {
        printf("AST успешно создан\n");

        graphviz(ast);
        //free_ast(ast);
    } 
    
    else 
    {
        printf("Не удалось создать AST\n");
    }

    free_parser(parser);
    free_lexer(lexer);

    printf("конец\n");

    return 0;
}

Lexer* lexer_creat(char* contest)
{
    if(!contest) return NULL;

    Lexer* lexer = (Lexer*)calloc(1, sizeof(Lexer));

    if(!lexer) return NULL;

    lexer -> count_symbol = 0;
    lexer -> contest = contest;
    lexer -> symbol = *contest;

    return lexer;
}

Lexer* lexer_assignment(Lexer* lexer)
{
    if (!lexer) return NULL;

    lexer ->count_symbol++;

    lexer -> symbol = lexer -> contest[lexer -> count_symbol];

    return lexer;
}

Token* next_lexer_token(Lexer* lexer)
{
    if(!lexer) return NULL;

    while(lexer->symbol != '\0')
    {
        if(lexer->symbol == ' ' || lexer->symbol == '\n')
        {
            lexer_skip(lexer);
            
            return next_lexer_token(lexer); //изменение вместо countinio
        }

        if(lexer->symbol == '"')
        {
            return lexer_string(lexer);
        }

        if(isalnum(lexer->symbol) || lexer->symbol == '_')
        {
            return create_token(TOKEN_ID, lexer_id(lexer));
        }

        //если надо остальные спецсимволы прописывать здесь
        switch(lexer->symbol)
        {
            case '=':
                return create_token(TOKEN_EQUALS, lexer_special_symbol(lexer));

            case ',':
                return create_token(TOKEN_COMMA, lexer_special_symbol(lexer));

            case ';':
                return create_token(TOKEN_SEMICOLON, lexer_special_symbol(lexer));

            case '{':
                return create_token(TOKEN_LPAREN, lexer_special_symbol(lexer));

            case '}':
                return create_token(TOKEN_RPAREN, lexer_special_symbol(lexer));

            case '(':
                return create_token(TOKEN_LBRACE, lexer_special_symbol(lexer));

            case ')':
                return create_token(TOKEN_RBRACE, lexer_special_symbol(lexer));

            case '+':
            case '-':
            case '*':
            case '/':
            {
                // Для операторов тоже создаем токены ID
                return create_token(TOKEN_ID, lexer_special_symbol(lexer));
            }

            default:
            {
                printf("ошибка в считывание токена: неожиданный символ '%c' (код: %d)\n", lexer -> symbol, lexer -> symbol);

                return create_token(TOKEN_EOF, "\0");
            }
        }
    }

    return create_token(TOKEN_EOF, "\0");
}

void lexer_skip(Lexer* lexer)
{
    if(!lexer) return;

    while(lexer->symbol == ' ' || lexer->symbol == '\n')
    {
        lexer_assignment(lexer);
    }
}

Token* lexer_string(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(1, sizeof(char));

    if(!value) return NULL;

    int count = 0;

    lexer_assignment(lexer); // Пропускаем первую кавычку

    while (lexer -> symbol != '"' && lexer -> symbol != '\0')
    {
        char* temp = (char*)realloc(value, (count + 2) * sizeof(char));

        if(!temp)
        {
            free(value);
            return NULL;
        }

        value = temp;

        value[count] = lexer -> symbol;

        lexer_assignment(lexer);
        count++;
    }

    value[count] = '\0';

    if(lexer->symbol == '"') 
    {
        lexer_assignment(lexer); // Пропускаем закрывающую кавычку
    }

    return create_token(TOKEN_STRING, value);
}

char* lexer_id(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(1, sizeof(char));

    int count = 0;

    while (isalnum(lexer->symbol) || lexer->symbol == '_')
    {
        char* temp = (char*)realloc(value, (count + 2) * sizeof(char));

        value = temp;

        value[count] = lexer -> symbol;

        lexer_assignment(lexer);
        count++;
    }

    value[count] = '\0';

    return value;
}

char* lexer_special_symbol(Lexer* lexer)
{
    if(!lexer) return NULL;

    char* value = (char*)calloc(2, sizeof(char));

    value[0] = lexer -> symbol;

    value[1] = '\0';

    lexer_assignment(lexer);

    return value;
}

Token* expected_token(Lexer* lexer, TYPE_TOKEN expected_type)
{
    Token* next_token = next_lexer_token(lexer);

    if(next_token && next_token -> type == expected_type)
    {
        return next_token;
    }
    else
    {
        printf("ошибка в логике следующий токен не совпадает с ожидаемым\n");

        if (next_token) 
        {
            free_token(next_token);
        }
        exit(1);
    }
}

Parser* inizial_parser(Lexer* lexer)
{
    if (!lexer) return NULL;

    Parser* parser = (Parser*)calloc(1, sizeof(Parser));

    parser -> lexer = lexer;
    parser -> token_now = NULL;
    parser -> token_prev = NULL;

    new_parser(parser);

    return parser;
}

Ast* parser_eof(Parser* parser)
{
    Ast* ast_tree = parser_opend(parser);

    if(parser -> token_now && parser -> token_now -> type != TOKEN_EOF)
    {
        printf("программа не наткнулась на EOF\n");

        if (ast_tree) free_ast(ast_tree);

        return NULL;
    }

    return ast_tree;
}

Ast* parser_opend(Parser* parser)
{
    Ast* ast_tree = parser_while(parser);
    
    // Если не EOF, продолжаем парсинг
    char* str = strdup("END_OP");
    
    new_parser(parser);

    if(parser -> token_now -> type == TOKEN_EOF)
    {
        char* str1 = strdup("END_PROG");

        Ast* ast_tree1 = creat_node(NULL, NULL, END_PROG, str1);

        return creat_node(ast_tree, ast_tree1, END_OP, str);
    }

    Lexer* lexer_copy = (Lexer*)calloc(1, sizeof(struct LEXER));

    memcpy(lexer_copy, parser -> lexer, sizeof(struct LEXER));

    Token* token_copy = next_lexer_token(lexer_copy);

    if(token_copy -> type == TOKEN_EOF || parser -> token_now -> type == TOKEN_RPAREN)
    {
        return creat_node(ast_tree, NULL, END_OP, str);
    }

    Ast* ast_tree1 = parser_opend(parser);

    return creat_node(ast_tree, ast_tree1, END_OP, str);
    // Не освобождаем str здесь - creat_node заберет владение
}

Ast* parser_while(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "while") == 0)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LBRACE) 
        {
            printf("Ожидалось '(' после while\n");
            free(str);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree = parser_log(parser);

        if(!ast_tree) 
        {
            free(str);
            return NULL;
        }

        if(parser -> token_now -> type != TOKEN_RBRACE) 
        {
            printf("Ожидалось ')' после условия while\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LPAREN) 
        {
            printf("Ожидалось '{' после условия while\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree2 = parser_opend(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        if (parser -> token_now -> type != TOKEN_RPAREN) 
        {
            printf("Ожидалось '}' после тела while\n");

            free(str);
            free_ast(ast_tree);
            free_ast(ast_tree2);

            return NULL;
        }

        //new_parser(parser);

        Ast* result = creat_node(ast_tree, ast_tree2, NODE_WHILE, str);
        // Не освобождаем str здесь - creat_node заберет владение
        return result;
    }
    else
    {
        return parser_if(parser);
    }
}

Ast* parser_if(Parser* parser)
{
    if(strcmp(parser -> token_now -> value, "if") == 0)
    {
        char* str = strdup("if_else");

        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LBRACE) 
        {
            printf("Ожидалось '(' после if\n");
            free(str);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree = parser_log(parser);

        if(!ast_tree) 
        {
            free(str);
            return NULL;
        }

        if(parser -> token_now -> type != TOKEN_RBRACE) 
        {
            printf("Ожидалось ')' после условия условия if\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

       
        new_parser(parser);

        if(parser -> token_now -> type != TOKEN_LPAREN) 
        {
            printf("Ожидалось '{' после условия if\n");

            free(str);
            free_ast(ast_tree);

            return NULL;
        }

        new_parser(parser);

        Ast* ast_tree1 = parser_opend(parser);

        if (!ast_tree1) 
        {
            free(str);
            free_ast(ast_tree);

            return NULL;
        }
        //printf("%s", parser -> token_now -> value);

        if(parser -> token_now -> type != TOKEN_RPAREN) 
        {
            printf("Ожидалось '}' после тела if\n");

            free(str);
            free_ast(ast_tree);
            free_ast(ast_tree1);

            return NULL;
        }

        new_parser(parser);

        if(strcmp(parser -> token_now -> value, "else") == 0) 
        {
            new_parser(parser);

            if(parser -> token_now -> type != TOKEN_LPAREN) 
            {
                printf("Ожидалось '{' после условия else\n");

                free(str);
                free_ast(ast_tree);

                return NULL;
            }

            new_parser(parser);

            Ast* ast_tree2 = parser_opend(parser);

            if(!ast_tree2) 
            {
                free(str);
                free_ast(ast_tree);

                return NULL;
            }

            if(parser -> token_now -> type != TOKEN_RPAREN) 
            {
                printf("Ожидалось '}' после тела else\n");

                free(str);
                free_ast(ast_tree);
                free_ast(ast_tree2);

                return NULL;
            }

            new_parser(parser);

            Ast* result = creat_node_if(ast_tree, ast_tree1, ast_tree2, str);

            return result;
        }
        else
        {
            printf("отсутствует else после if");

            return NULL;
        }
    }
    else
    {
        return parser_log(parser);
    }
}

Ast* parser_log(Parser* parser)
{

    Ast* ast_tree = parser_bin(parser);

    if(parser -> token_now && parser -> token_now -> value &&
       strcmp(parser -> token_now -> value, "or") == 0)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);
            return NULL;
        }

        ast_tree = creat_node(ast_tree, ast_tree2, LOGIC_OPTION, str);
        // Не освобождаем str здесь - creat_node заберет владение
    }

    return ast_tree;
}

Ast* parser_bin(Parser* parser)
{
    Ast* ast_tree = parser_equal(parser);

    if(is_operator(parser -> token_now -> value) == 1) 
    {
        char* str = strdup(parser->token_now->value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if(!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree);
            return NULL;
        }

        ast_tree = creat_node(ast_tree, ast_tree2, BINARY_OPTION, str);
        // Не освобождаем str здесь - creat_node заберет владение
    }

    return ast_tree;
}

Ast* parser_equal(Parser* parser)
{
    Ast* ast_tree = parser_dif_var(parser);//тут должен быть dif_var

    if(!ast_tree) return NULL;

    if(parser->token_now && parser->token_now->type == TOKEN_EQUALS)
    {
        char* str = strdup(parser -> token_now -> value);

        new_parser(parser);

        Ast* ast_tree2 = parser_bin(parser);

        if (!ast_tree2) 
        {
            free(str);
            free_ast(ast_tree); 

            return NULL;
        }

        Ast* ast_tree3 = creat_node(ast_tree, ast_tree2, EQUAL, str);

        return ast_tree3;
        // Не освобождаем str здесь - creat_node заберет владение
    }

    return ast_tree;
}

Ast* parser_dif_var(Parser* parser)
{
    if(parser -> token_now -> value && strcmp(parser -> token_now -> value, "var") == 0)
    {
        new_parser(parser);

        Ast* ast_tree = parser_var(parser);

        if(ast_tree) 
        {
            ast_tree -> type = VARIABLE_DEFINITION;
        }

        return ast_tree;
    }
    else
    {
        return parser_var(parser);
    }
}

Ast* parser_var(Parser* parser)
{
    if(has_no_digits_simple(parser -> token_now -> value) &&
       parser -> token_now -> type != TOKEN_STRING)
    {
        Ast* ast_node = creat_node(NULL, NULL, VARIBLE, strdup(parser -> token_now -> value));

        new_parser(parser);

        return ast_node;
    }
    else
    {
        return parser_const(parser);
    }
}

Ast* parser_const(Parser* parser)
{
    if(parser -> token_now -> type == TOKEN_ID || parser -> token_now -> type == TOKEN_STRING)
    {
        char* token_value = strdup(parser -> token_now -> value);

        Ast* ast_node = creat_node(NULL, NULL, CONS, token_value);

        new_parser(parser);

        return ast_node;
    }
    else
    {
        printf("ошибка в синтаксисе спустились до низу и нашлось совпадений %s\n", parser -> token_now -> value);

        return NULL;
    }
}

Parser* new_parser(Parser* parser)
{
    // Освобождаем только token_prev, если он есть
    if(parser -> token_prev) 
    {
        free_token(parser -> token_prev);

        parser -> token_prev = NULL;
    }

    // Перемещаем token_now в token_prev
    parser -> token_prev = parser -> token_now;
    
    // Получаем новый токен
    parser -> token_now = next_lexer_token(parser -> lexer);

    printf("TOKEN (%s)\n", parser -> token_now -> value);

    return parser;
}

bool has_no_digits_simple(const char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]) || str[i] == '_')
        {
            return true;
        }
    }
    return false;
}

Ast* creat_node(Ast* ast_tree1, Ast* ast_tree2, PARSER_TYPE type, char* str)
{
    Ast* ast_node = (Ast*)calloc(1, sizeof(Ast));

    ast_node -> type = type;

    switch (type)
    {
        case EQUAL:
        {
            ast_node -> assignment.assignment_name = str;
            ast_node -> assignment.left = ast_tree1;
            ast_node -> assignment.right = ast_tree2;
            break;
        }
        case VARIBLE:
        {
            ast_node -> varible.var = str;
            ast_node -> varible.node_left_var = ast_tree1;
            ast_node -> varible.node_right_var = ast_tree2;
            break;
        }
        case CONS:
        {
            char* copy = str;

            ast_node -> cons.string = copy;
            ast_node -> cons.node_left_cons = ast_tree1;
            ast_node -> cons.node_right_cons = ast_tree2;
            break;
        }
        case BINARY_OPTION:
        {
            ast_node -> bin_op.binar_oper = str;
            ast_node -> bin_op.node_left_bin = ast_tree1;
            ast_node -> bin_op.node_right_bin = ast_tree2;
            break;
        }
        case LOGIC_OPTION:
        {
            ast_node -> log_op.logic_oper = str;
            ast_node -> log_op.node_left_log = ast_tree1;
            ast_node -> log_op.node_right_log = ast_tree2;
            break;
        }
        case VARIABLE_DEFINITION:
        {
            ast_node -> declaring_variable.name = str;
            ast_node -> declaring_variable.left = ast_tree1;
            ast_node -> declaring_variable.right = ast_tree2;
            break;
        }
        case NODE_WHILE:
        {
            ast_node -> fun_while.fun_while_oper = str;
            ast_node -> fun_while.condition = ast_tree1;
            ast_node -> fun_while.body = ast_tree2;
            break;
        }
        case END_OP:
        {
            ast_node -> end_op.end_op_name = str;
            ast_node -> end_op.left = ast_tree1;
            ast_node -> end_op.right = ast_tree2;
            break;
        }
        case END_PROG:
        {
            ast_node -> end_prog.end_prog_name = str;
            ast_node -> end_prog.left = ast_tree1;
            ast_node -> end_prog.right = ast_tree2;
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

    ast_node -> fun_if.if_else_oper = str;
    ast_node -> fun_if.if_condition = ast_tree;
    ast_node -> fun_if.if_body = ast_tree1;
    ast_node -> fun_if.else_body = ast_tree2;

    return ast_node;
}

Token* create_token(TYPE_TOKEN type, char* value)
{
    Token* token = (Token*)calloc(1, sizeof(Token));

    token -> type = type;
    token -> value = value;

    return token;
}

void free_ast(Ast* ast)
{
    switch (ast -> type)
    {
        case EQUAL:
        {
            if(ast -> assignment.assignment_name) free(ast->assignment.assignment_name);

            free_ast(ast -> assignment.left);
            free_ast(ast -> assignment.right);
            break;
        }
        case VARIBLE:
        {
            if(ast -> varible.var) free(ast -> varible.var);

            free_ast(ast -> varible.node_left_var);
            free_ast(ast -> varible.node_right_var);
            break;
        }
        case CONS:
        {
            if(ast->cons.string) free(ast -> cons.string);

            free_ast(ast -> cons.node_left_cons);
            free_ast(ast -> cons.node_right_cons);
            break;
        }
        case BINARY_OPTION:
        {
            if(ast -> bin_op.binar_oper) free(ast -> bin_op.binar_oper);

            free_ast(ast -> bin_op.node_left_bin);
            free_ast(ast -> bin_op.node_right_bin);
            break;
        }
        case LOGIC_OPTION:
        {
            if(ast -> log_op.logic_oper) free(ast -> log_op.logic_oper);

            free_ast(ast -> log_op.node_left_log);
            free_ast(ast -> log_op.node_right_log);
            break;
        }
        case VARIABLE_DEFINITION:
        {
            if(ast -> declaring_variable.name) free(ast -> declaring_variable.name);

            free_ast(ast -> declaring_variable.left);
            free_ast(ast -> declaring_variable.right);
            break;
        }
        case NODE_WHILE:
        {
            if(ast -> fun_while.fun_while_oper) free(ast -> fun_while.fun_while_oper);

            free_ast(ast -> fun_while.condition);
            free_ast(ast -> fun_while.body);
            break;
        }
        case END_OP:
        {
            if(ast -> end_op.end_op_name) free(ast -> end_op.end_op_name);

            free_ast(ast -> end_op.left);
            free_ast(ast -> end_op.right);
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

int graphviz(Ast* root)
{
    if (!root) {
        printf("AST пустой, невозможно создать graphviz\n");
        return 1;
    }
     
    FILE *file = fopen("graphviz.dot", "w");

    if (file == NULL)
    {
        printf("Ошибка открытия файла\n");
        return 1;
    }

    fprintf(file, "digraph {\n");
    fprintf(file, "    rankdir=TB;\n");
    fprintf(file, "    node [shape=none];\n");
    fprintf(file, "    edge [color=blue];\n\n");

    made_graphviz_dot(root, file);

    fprintf(file, "\n");
    fprintf(file, "}\n");

    fclose(file);
    
    printf("Graphviz файл создан: graphviz.dot\n");
    printf("Для визуализации выполните: dot -Tpng graphviz.dot -o ast.png\n");

    return 0;
}

int made_graphviz_dot(Ast* root, FILE* file)
{
    if(root == NULL)
    {
        return 0;
    }

    fprintf(file, "    node_%p [label=<\n", root);
    fprintf(file, "        <table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
    fprintf(file, "        <tr>\n");

    if(root -> type == END_OP)
    {
        const char* label = root->end_op.end_op_name ? root->end_op.end_op_name : "END_OP";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");  
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> end_op.left != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> end_op.left);

            made_graphviz_dot(root -> end_op.left, file);
        }

        if(root -> end_op.right != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> end_op.right);

            made_graphviz_dot(root -> end_op.right, file);
        }
    }

    if(root -> type == END_PROG)
    {
        const char* label = root->end_prog.end_prog_name ? root->end_prog.end_prog_name : "END_PROG";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");  
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> end_prog.left != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> end_prog.left);

            made_graphviz_dot(root -> end_prog.left, file);
        }

        if(root -> end_prog.right != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> end_prog.right);

            made_graphviz_dot(root -> end_prog.right, file);
        }
    }

    else if(root -> type == VARIABLE_DEFINITION)
    {
        const char* label = root->varible.var ? root->varible.var : "VAR_DEF";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> varible.node_left_var != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> varible.node_left_var);

            made_graphviz_dot(root -> varible.node_left_var, file);
        }

        if(root -> varible.node_right_var != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> varible.node_right_var);

            made_graphviz_dot(root -> varible.node_right_var, file);
        }
    }
    
    else if(root -> type == VARIBLE)
    {
        const char* label = root->varible.var ? root->varible.var : "VAR";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> varible.node_left_var != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> varible.node_left_var);

            made_graphviz_dot(root -> varible.node_left_var, file);
        }

        if(root -> varible.node_right_var != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> varible.node_right_var);

            made_graphviz_dot(root -> varible.node_right_var, file);
        }
    }
    
    else if(root -> type == CONS)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root->cons.string);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> cons.node_left_cons != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> cons.node_left_cons);

            made_graphviz_dot(root -> cons.node_left_cons, file);
        }

        if(root -> cons.node_right_cons != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> cons.node_right_cons);

            made_graphviz_dot(root -> cons.node_right_cons, file);
        }
    }
    
    else if(root -> type == EQUAL)
    {
        const char* label = root->assignment.assignment_name ? root->assignment.assignment_name : "=";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> assignment.left != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> assignment.left);

            made_graphviz_dot(root -> assignment.left, file);
        }

        if(root -> assignment.right != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> assignment.right);

            made_graphviz_dot(root -> assignment.right, file);
        }
    }
    
    else if(root -> type == BINARY_OPTION)
    {
        const char* label = root->bin_op.binar_oper ? root->bin_op.binar_oper : "BIN_OP";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> bin_op.node_left_bin != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> bin_op.node_left_bin);

            made_graphviz_dot(root -> bin_op.node_left_bin, file);
        }

        if(root -> bin_op.node_right_bin != NULL)
        {   
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> bin_op.node_right_bin);

            made_graphviz_dot(root -> bin_op.node_right_bin, file);
        }
    }
    
    else if(root -> type == LOGIC_OPTION)
    {
        const char* label = root->log_op.logic_oper ? root->log_op.logic_oper : "LOGIC_OP";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> log_op.node_left_log != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> log_op.node_left_log);

            made_graphviz_dot(root -> log_op.node_left_log, file);
        }

        if(root -> log_op.node_right_log != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> log_op.node_right_log);

            made_graphviz_dot(root -> log_op.node_right_log, file);
        }
    }
    
    else if(root -> type == NODE_WHILE)
    {
        const char* label = root->fun_while.fun_while_oper ? root->fun_while.fun_while_oper : "while";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> fun_while.condition != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> fun_while.condition);

            made_graphviz_dot(root -> fun_while.condition, file);
        }

        if(root -> fun_while.body != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> fun_while.body);

            made_graphviz_dot(root -> fun_while.body, file);
        }
    }
    else if(root -> type == NODE_IF)
    {
        const char* label = root->fun_if.if_else_oper ? root->fun_if.if_else_oper : "if_else";
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", label);
        fprintf(file, "        </tr>\n");
        fprintf(file, "     </table>\n");
        fprintf(file, "     >];\n");

        if(root -> fun_if.if_condition != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> fun_if.if_condition);

            made_graphviz_dot(root -> fun_if.if_condition, file);
        }

        if(root -> fun_if.if_body != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> fun_if.if_body);

            made_graphviz_dot(root -> fun_if.if_body, file);
        }

        if(root -> fun_if.else_body != NULL)
        {
            fprintf(file, "    node_%p -> node_%p;\n",
            root, root -> fun_if.else_body);

            made_graphviz_dot(root -> fun_if.else_body, file);
        }
    }

    return 0;
}

int is_operator(char* str) 
{   
    switch (str[0]) 
    {
        case '+':
        case '-':
        case '*':
        case '/':
            return 1;

        default:
            return 0;
    }
}

void compile_to_c(Ast* root)
{
    FILE* file = fopen("main.c", "w");
    
    fprintf(file, "#include stdio.h\n");

    //interprit(root, file);

    char command[256];

    sprintf(command, "g++ main.c -o program.exe");
    system(command);
}

void interprit(Ast* root, FILE* file)
{
}











char* read_file_without_newlines()
{
    FILE* file = fopen("text.txt", "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        return strdup("");  // Пустая строка для пустого файла
    }

    // Выделяем память: размер файла + 1 для '\0'
    char* str = (char*)malloc(file_size + 1);
    if (!str) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    int index = 0;
    int c;
    
    // Читаем файл посимвольно
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            // Заменяем \n на пробел
            str[index++] = ' ';
        } else {
            str[index++] = (char)c;
        }
    }
    
    str[index] = '\0';  // Завершаем строку
    
    fclose(file);
    return str;
}