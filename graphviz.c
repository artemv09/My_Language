#include "graphviz.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int graphviz(Ast* root)
{
    if (!root) 
    {
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
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> end_op_name);
    }

    else if(root -> type == END_PROG)
    { 
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> end_prog_name);
    }

    else if(root -> type == VARIABLE_DEFINITION)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }
    
    else if(root -> type == VARIBLE)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }
    
    else if(root -> type == CONS)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> string);
    }
    
    else if(root -> type == EQUAL)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> assignment_name);
    }

    else if(root -> type == FUNC_CALL)
    { 
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }

    else if(root -> type == FUNC_DEF)
    { 
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }

    else if(root -> type == RETURN)
    { 
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }
    
    else if(root -> type == BINARY_OPTION)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> binar_oper);
    }
    
    else if(root -> type == LOGIC_OPTION)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> logic_oper);
    }
    
    else if(root -> type == NODE_WHILE)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> fun_while_oper);
    }

    else if(root -> type == NODE_IF)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> if_else_oper);
    }

    else if(root -> type == PRINT)
    {
        fprintf(file, "                <td colspan=\"2\" align=\"center\">%s</td>", root -> var);
    }

    fprintf(file, "        </tr>\n");
    fprintf(file, "     </table>\n");
    fprintf(file, "     >];\n");

    if(root -> left != NULL)
    {
        fprintf(file, "    node_%p -> node_%p;\n", root, root -> left);

        made_graphviz_dot(root -> left, file);
    }

    if(root -> right != NULL)
    {
        fprintf(file, "    node_%p -> node_%p;\n", root, root -> right);

        made_graphviz_dot(root -> right, file);
    }

    if(root -> type == NODE_IF && root -> else_body != NULL)
    {
        fprintf(file, "    node_%p -> node_%p;\n", root, root -> else_body);

        made_graphviz_dot(root -> else_body, file);
    }

    return 0;
}
