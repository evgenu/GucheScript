#include "parser.h"

void advance(parser_t *parser) {
    parser->index++;
}

token_t *current_token(parser_t *parser) {
    return &parser->tokens[parser->index];
}

token_t *peek_next_token(parser_t *parser) {
    return &parser->tokens[parser->index + 1];
}

void consume(parser_t *parser, token_types type) {
    if (current_token(parser)->type == type) {
        advance(parser);
    } else {
        printf("Error: Unexpected token %d at position %zu\n", current_token(parser)->type, parser->index);
        exit(1);
    }
}

ASTNode *parse_function(parser_t *parser) {
    consume(parser, TOKEN_FUNC);
    
    if (current_token(parser)->type != TOKEN_NAME) {
        printf("Error: Expected function name\n");
        exit(1);
    }
    
    char *func_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_OCURLY);

    int count = 0;
    ASTNode **body = (ASTNode **)malloc(1 * sizeof(ASTNode *));

    while (current_token(parser)->type != TOKEN_CCURLY && current_token(parser)->type != TOKEN_EOF) {
        count++;
        body = (ASTNode**)realloc(body, count * sizeof(ASTNode *));
        ASTNode *stmt = parse_statement(parser);
        body[count - 1] = stmt;
    }

    consume(parser, TOKEN_CCURLY);

    return create_node(AST_FUNCTION, func_name, 0, body, count);
}

ASTNode *parse_statement(parser_t *parser) {
    if (current_token(parser)->type == TOKEN_INT || current_token(parser)->type == TOKEN_CHAR) {
        return parse_variable_declaration(parser);
    } else if (current_token(parser)->type == TOKEN_NAME) {
        if (peek_next_token(parser)->type == TOKEN_OPAREN) {
            return parse_function_call(parser); 
        } else if (peek_next_token(parser)->type == TOKEN_ASSIGN) {
            return parse_assignment(parser);  
        }
    } else if (current_token(parser)->type == TOKEN_IF) {
        return parse_if_statement(parser);
    } else if (current_token(parser)->type == TOKEN_FUNC) {
        return parse_function(parser);  
    } else {
        printf("Error: Invalid statement\n");
        exit(1);
    }
}

ASTNode *parse_variable_declaration(parser_t *parser) {
    token_types var_type = current_token(parser)->type;
    advance(parser);

    if (current_token(parser)->type != TOKEN_NAME) {
        exit(1);
    }

    char *var_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_ASSIGN);

    ASTNode *expr = parse_expression(parser);
    ASTNode *assignement = create_node(AST_ASSIGNMENT, "=", 0, &expr, 1);
    return create_node(AST_VAR_DECL, var_name, 0, &assignement, 1);
}

ASTNode *parse_assignment(parser_t *parser) {
    char *var_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_ASSIGN);

    ASTNode *expr = parse_expression(parser);
    ASTNode *assignement = create_node(AST_ASSIGNMENT, "=", 0, &expr, 1);
    return create_node(AST_ASSIGNMENT, var_name, 0, &assignement, 1);
}

ASTNode *parse_expression(parser_t *parser) {
    ASTNode *left = NULL;

    if (current_token(parser)->type == TOKEN_NUMBER) {
        left = create_node(AST_LITERAL, NULL, atoi(current_token(parser)->value), NULL, 0);
    } else if (current_token(parser)->type == TOKEN_NAME) {
        left = create_node(AST_IDENTIFIER, current_token(parser)->value, 0, NULL, 0);
    } else {
        printf("Error: Invalid expression\n");
        exit(1);
    }

    advance(parser);

    while (current_token(parser)->type == TOKEN_PLUS ||
           current_token(parser)->type == TOKEN_MINUS ||
           current_token(parser)->type == TOKEN_MUL ||
           current_token(parser)->type == TOKEN_DIV) {
        token_t *op_token = current_token(parser);
        advance(parser);

        ASTNode *right = parse_expression(parser);
        ASTNode **children = (ASTNode **)malloc(2 * sizeof(ASTNode *));
        children[0] = left;
        children[1] = right;
        left = create_node(AST_EXPRESSION, op_token->value, 0, children, 2);
    }

    while (current_token(parser)->type == TOKEN_GREATER ||
           current_token(parser)->type == TOKEN_LESS) {
        token_t *op_token = current_token(parser);
        advance(parser);

        ASTNode *right = parse_expression(parser);

        ASTNode **children = (ASTNode **)malloc(2 * sizeof(ASTNode *));
        children[0] = left;
        children[1] = right;
        left = create_node(AST_EXPRESSION, op_token->value, 0, children, 2);
    }

    return left;
}

ASTNode *parse_if_statement(parser_t *parser) {
    consume(parser, TOKEN_IF);

    consume(parser, TOKEN_OPAREN);
    ASTNode *condition = parse_expression(parser);
    consume(parser, TOKEN_CPAREN);

    consume(parser, TOKEN_OCURLY);

    int count = 0;
    ASTNode **body = (ASTNode **)malloc(1 * sizeof(ASTNode *));

    count++;
    body = (ASTNode**)realloc(body, count * sizeof(ASTNode *));
    body[count - 1] = condition;

    while (current_token(parser)->type != TOKEN_CCURLY && current_token(parser)->type != TOKEN_EOF) {
        count++;
        body = (ASTNode**)realloc(body, count * sizeof(ASTNode *));
        ASTNode *stmt = parse_statement(parser);
        body[count - 1] = stmt;
    }

    consume(parser, TOKEN_CCURLY);

    return create_node(AST_IF_STATEMENT, NULL, 0, body, count);
}

ASTNode *parse_function_call(parser_t *parser) {
    if (current_token(parser)->type != TOKEN_NAME) {
        printf("Error: Expected function name\n");
        exit(1);
    }

    char *func_name = strdup(current_token(parser)->value);
    advance(parser);

    consume(parser, TOKEN_OPAREN); 

    ASTNode *args = NULL;
    ASTNode *last_arg = NULL;

    if (current_token(parser)->type != TOKEN_CPAREN) {
        args = parse_expression(parser);
        last_arg = args;
    }

    consume(parser, TOKEN_CPAREN);  

    return create_node(AST_FUNC_CALL, func_name, 0, &(args), 1);
}
