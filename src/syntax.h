#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

typedef struct ASTNode {
    ASTNodeType type;
    char *name;
    int value;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *create_node(ASTNodeType type, char *name, int value, ASTNode *left, ASTNode *right);
void print_ast(ASTNode *node, int level);


#endif // SYNTAX_H