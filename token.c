#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Token* create_token(TYPE_TOKEN type, char* value)
{
    Token* token = (Token*)calloc(1, sizeof(struct Token));

    token -> type = type;
    token -> value = value;

    return token;
}