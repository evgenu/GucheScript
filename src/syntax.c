#include "syntax.h"

ASTNode *create_node(ASTNodeType type, char *name, int value, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->name = name ? strdup(name) : NULL;
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}


void print_ast(ASTNode *node, int level)
{
    if (node == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    switch (node->type)
    {
    case AST_FUNCTION:
        printf("Function: %s\n", node->name);
        break;

    case AST_VAR_DECL:
        printf("Variable Declaration: %s\n", node->name);
        break;

    case AST_ASSIGNMENT:
        printf("Assignment: %s\n", node->name);
        break;

    case AST_EXPRESSION:
        printf("Expression: %s\n", node->name);
        break;

    case AST_IF_STATEMENT:
        printf("If Statement\n");
        break;

    case AST_FUNC_CALL:
        printf("Function Call: %s\n", node->name);
        break;

    case AST_LITERAL:
        printf("Literal: %d\n", node->value);
        break;

    case AST_IDENTIFIER:
        printf("Identifier: %s\n", node->name);
        break;

    default:
        printf("Unknown node type\n");
        break;
    }

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}
