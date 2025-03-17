#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "lexer.h"
#include "syntax.h"

typedef struct parser_t
{
    token_t *tokens;
    size_t index;
} parser_t;

void advance(parser_t *parser);
void consume(parser_t *parser, token_types type);
token_t *current_token(parser_t *parser);
token_t *peek_next_token(parser_t *parser);
ASTNode *parse_function(parser_t *parser);
ASTNode *parse_statement(parser_t *parser);
ASTNode *parse_variable_declaration(parser_t *parser);
ASTNode *parse_assignment(parser_t *parser);
ASTNode *parse_expression(parser_t *parser);
ASTNode *parse_if_statement(parser_t *parser);
ASTNode *parse_function_call(parser_t *parser);

#endif // PARSER_H