#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "AST.h"
#include <stdio.h>


int graphviz(Ast* root);

int made_graphviz_dot(Ast* root, FILE* file);

#endif