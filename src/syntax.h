#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

typedef struct ASTNode
{
    ast_node_type type;
    char *name;
    int value;
    int current;
    int size;
    struct ast_node_t **children;
} ast_node_t;

ast_node_t *create_node(ast_node_type type, char *name, int value, ast_node_t **children, int count);
void print_ast(ast_node_t *node, int level);

#endif // SYNTAX_H