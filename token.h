#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    TOKEN_ID, // индификатор пока что туда пихнем все
    TOKEN_STRING,
    TOKEN_EQUALS, // равно
    TOKEN_COMMA, // запятая
    TOKEN_SEMICOLON, //точка с зяпятой
    TOKEN_LPAREN, // {
    TOKEN_RPAREN,
    TOKEN_LBRACE, // 
    TOKEN_RBRACE,
    TOKEN_EOF
    
} TYPE_TOKEN;

typedef struct Token
{
    TYPE_TOKEN type;
    char* value;

} Token;

Token* create_token(TYPE_TOKEN type, char* value);

#endif