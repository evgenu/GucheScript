#include "syntax.h"

ast_node_t *create_node(ast_node_type type, char *name, int value, ast_node_t **children, int count)
{
    ast_node_t *node = (ast_node_t *)malloc(sizeof(ast_node_t));
    node->type = type;
    node->name = name ? strdup(name) : NULL;
    node->value = value;
    node->children = (ast_node_t **)malloc(count * sizeof(ast_node_t *));
    for (int i = 0; i < count; i++)
    {
        node->children[i] = children[i];
    }
    node->size = count;
    node->current = 0;
    return node;
}

void print_ast(ast_node_t *node, int level)
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

    for (int i = 0; i < node->size; i++)
    {
        print_ast(node->children[i], level + 1);
    }
}
