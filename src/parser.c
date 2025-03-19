#include "parser.h"

void advance(parser_t *parser)
{
    parser->index++;
}

token_t *current_token(parser_t *parser)
{
    return &parser->tokens[parser->index];
}

token_t *peek_next_token(parser_t *parser)
{
    return &parser->tokens[parser->index + 1];
}

void consume(parser_t *parser, token_types type)
{
    if (current_token(parser)->type == type)
    {
        advance(parser);
    }
    else
    {
        printf("Error: Unexpected token %d with value %s at position %zu\n", current_token(parser)->type, current_token(parser)->value, parser->index);
        exit(1);
    }
}

int precedence(token_types type)
{
    switch (type)
    {
    case TOKEN_EQUAL:
        return 1;
    case TOKEN_LESS:
    case TOKEN_GREATER:
        return 2;
    case TOKEN_PLUS:
    case TOKEN_MINUS:
        return 3;
    case TOKEN_MUL:
    case TOKEN_DIV:
        return 4;
    default:
        return 0;
    }
}

ast_node_t *parse_function(parser_t *parser)
{
    consume(parser, TOKEN_FUNC);

    if (current_token(parser)->type != TOKEN_NAME)
    {
        printf("Error: Expected function name\n");
        exit(1);
    }

    char *func_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_OCURLY);

    int count = 0;
    ast_node_t **body = (ast_node_t **)malloc(1 * sizeof(ast_node_t *));

    while (current_token(parser)->type != TOKEN_CCURLY && current_token(parser)->type != TOKEN_EOF)
    {
        count++;
        body = (ast_node_t **)realloc(body, count * sizeof(ast_node_t *));
        ast_node_t *stmt = parse_statement(parser);
        body[count - 1] = stmt;
    }

    consume(parser, TOKEN_CCURLY);

    return create_node(AST_FUNCTION, func_name, 0, body, count);
}

ast_node_t *parse_statement(parser_t *parser)
{
    if (current_token(parser)->type == TOKEN_INT || current_token(parser)->type == TOKEN_CHAR)
    {
        return parse_variable_declaration(parser);
    }
    else if (current_token(parser)->type == TOKEN_NAME)
    {
        if (peek_next_token(parser)->type == TOKEN_OPAREN)
            return parse_function_call(parser);
        else if (peek_next_token(parser)->type == TOKEN_ASSIGN)
            return parse_assignment(parser);
    }
    else if (current_token(parser)->type == TOKEN_IF)
    {
        return parse_if_statement(parser);
    }
    else if (current_token(parser)->type == TOKEN_WHILE)
    {
        return parse_while_statement(parser);
    }
    else if (current_token(parser)->type == TOKEN_FUNC)
    {
        return parse_function(parser);
    }
    else
    {
        printf("Error: Invalid statement\n");
        exit(1);
    }
}

ast_node_t *parse_variable_declaration(parser_t *parser)
{
    token_types var_type = current_token(parser)->type;
    advance(parser);

    if (current_token(parser)->type != TOKEN_NAME)
        exit(1);

    char *var_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_ASSIGN);

    ast_node_t *expr = parse_expression(parser);
    ast_node_t *assignement = create_node(AST_ASSIGNMENT, "=", 0, &expr, 1);
    return create_node(AST_VAR_DECL, var_name, 0, &assignement, 1);
}

ast_node_t *parse_assignment(parser_t *parser)
{
    char *var_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_ASSIGN);

    ast_node_t *expr = parse_expression(parser);
    ast_node_t *assignement = create_node(AST_ASSIGNMENT, "=", 0, &expr, 1);
    return create_node(AST_ASSIGNMENT, var_name, 0, &assignement, 1);
}

ast_node_t *parse_expression(parser_t *parser)
{
    ast_node_t **output = NULL;
    int output_count = 0;
    token_t **operators = NULL;
    int operators_count = 0;
    int brackets = 0;

    while (current_token(parser)->type == TOKEN_ASSIGN ||
           current_token(parser)->type == TOKEN_NAME ||
           current_token(parser)->type == TOKEN_NUMBER ||
           current_token(parser)->type == TOKEN_OPAREN ||
           current_token(parser)->type == TOKEN_CPAREN ||
           current_token(parser)->type == TOKEN_PLUS ||
           current_token(parser)->type == TOKEN_MINUS ||
           current_token(parser)->type == TOKEN_MUL ||
           current_token(parser)->type == TOKEN_DIV ||
           current_token(parser)->type == TOKEN_GREATER ||
           current_token(parser)->type == TOKEN_LESS ||
           current_token(parser)->type == TOKEN_EQUAL)
    {
        if (current_token(parser)->type == TOKEN_NUMBER)
        {
            ast_node_t *number = create_node(AST_LITERAL, NULL, atoi(current_token(parser)->value), NULL, 0);
            output_count++;
            output = (ast_node_t **)realloc(output, output_count * sizeof(ast_node_t *));
            output[output_count - 1] = number;
            advance(parser);
        }
        else if (current_token(parser)->type == TOKEN_NAME)
        {
            ast_node_t *identifier = create_node(AST_IDENTIFIER, current_token(parser)->value, 0, NULL, 0);
            output_count++;
            output = (ast_node_t **)realloc(output, output_count * sizeof(ast_node_t *));
            output[output_count - 1] = identifier;
            advance(parser);
        }
        else if (current_token(parser)->type == TOKEN_OPAREN)
        {
            brackets++;
            operators_count++;
            operators = (token_t **)realloc(operators, operators_count * sizeof(token_t *));
            operators[operators_count - 1] = current_token(parser);
            advance(parser);
        }
        else if (current_token(parser)->type == TOKEN_CPAREN)
        {
            while (operators_count > 0 && operators[operators_count - 1]->type != TOKEN_OPAREN)
            {
                token_t *op = operators[--operators_count];
                ast_node_t *right = output[--output_count];
                ast_node_t *left = output[--output_count];
                ast_node_t **children = (ast_node_t **)malloc(2 * sizeof(ast_node_t *));
                children[0] = left;
                children[1] = right;
                ast_node_t *expr = create_node(AST_EXPRESSION, op->value, 0, children, 2);
                output_count++;
                output = (ast_node_t **)realloc(output, output_count * sizeof(ast_node_t *));
                output[output_count - 1] = expr;
            }
            operators_count--;
            if (brackets <= 0)
                break;
            advance(parser);
            brackets--;
        }
        else if (current_token(parser)->type == TOKEN_PLUS ||
                 current_token(parser)->type == TOKEN_MINUS ||
                 current_token(parser)->type == TOKEN_MUL ||
                 current_token(parser)->type == TOKEN_DIV ||
                 current_token(parser)->type == TOKEN_GREATER ||
                 current_token(parser)->type == TOKEN_LESS ||
                 current_token(parser)->type == TOKEN_EQUAL)
        {
            while (operators_count > 0 &&
                   precedence(operators[operators_count - 1]->type) >= precedence(current_token(parser)->type))
            {
                token_t *op = operators[--operators_count];
                ast_node_t *right = output[--output_count];
                ast_node_t *left = output[--output_count];
                ast_node_t **children = (ast_node_t **)malloc(2 * sizeof(ast_node_t *));
                children[0] = left;
                children[1] = right;
                ast_node_t *expr = create_node(AST_EXPRESSION, op->value, 0, children, 2);
                output_count++;
                output = (ast_node_t **)realloc(output, output_count * sizeof(ast_node_t *));
                output[output_count - 1] = expr;
            }
            operators_count++;
            operators = (token_t **)realloc(operators, operators_count * sizeof(token_t *));
            operators[operators_count - 1] = current_token(parser);
            advance(parser);
        }
        else
        {
            break;
        }
    }

    while (operators_count > 0)
    {
        token_t *op = operators[--operators_count];
        ast_node_t *right = output[--output_count];
        ast_node_t *left = output[--output_count];
        ast_node_t **children = (ast_node_t **)malloc(2 * sizeof(ast_node_t *));
        children[0] = left;
        children[1] = right;
        ast_node_t *expr = create_node(AST_EXPRESSION, op->value, 0, children, 2);
        output_count++;
        output = (ast_node_t **)realloc(output, output_count * sizeof(ast_node_t *));
        output[output_count - 1] = expr;
    }

    ast_node_t *result = output[0];
    free(output);
    free(operators);
    return result;
}

ast_node_t *parse_if_statement(parser_t *parser)
{
    consume(parser, TOKEN_IF);

    consume(parser, TOKEN_OPAREN);
    ast_node_t *condition = parse_expression(parser);
    consume(parser, TOKEN_CPAREN);

    consume(parser, TOKEN_OCURLY);

    int count = 0;
    ast_node_t **body = (ast_node_t **)malloc(1 * sizeof(ast_node_t *));

    count++;
    body = (ast_node_t **)realloc(body, count * sizeof(ast_node_t *));
    body[count - 1] = condition;

    while (current_token(parser)->type != TOKEN_CCURLY && current_token(parser)->type != TOKEN_EOF)
    {
        count++;
        body = (ast_node_t **)realloc(body, count * sizeof(ast_node_t *));
        ast_node_t *stmt = parse_statement(parser);
        body[count - 1] = stmt;
    }

    consume(parser, TOKEN_CCURLY);

    return create_node(AST_IF_STATEMENT, NULL, 0, body, count);
}

ast_node_t *parse_while_statement(parser_t *parser)
{
    consume(parser, TOKEN_WHILE);

    consume(parser, TOKEN_OPAREN);
    ast_node_t *condition = parse_expression(parser);
    consume(parser, TOKEN_CPAREN);

    consume(parser, TOKEN_OCURLY);

    int count = 0;
    ast_node_t **body = (ast_node_t **)malloc(1 * sizeof(ast_node_t *));

    count++;
    body = (ast_node_t **)realloc(body, count * sizeof(ast_node_t *));
    body[count - 1] = condition;

    while (current_token(parser)->type != TOKEN_CCURLY && current_token(parser)->type != TOKEN_EOF)
    {
        count++;
        body = (ast_node_t **)realloc(body, count * sizeof(ast_node_t *));
        ast_node_t *stmt = parse_statement(parser);
        body[count - 1] = stmt;
    }

    consume(parser, TOKEN_CCURLY);

    return create_node(AST_WHILE_STATEMENT, NULL, 0, body, count);
}

ast_node_t *parse_function_call(parser_t *parser)
{
    if (current_token(parser)->type != TOKEN_NAME)
    {
        printf("Error: Expected function name\n");
        exit(1);
    }

    char *func_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_OPAREN);

    ast_node_t *args = NULL;
    ast_node_t *last_arg = NULL;

    if (current_token(parser)->type != TOKEN_CPAREN)
    {
        args = parse_expression(parser);
        last_arg = args;
    }

    consume(parser, TOKEN_CPAREN);

    return create_node(AST_FUNC_CALL, func_name, 0, &(args), 1);
}
