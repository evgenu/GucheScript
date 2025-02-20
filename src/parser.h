#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "lexer.h"

typedef struct Parser {
    token_t * token_list;
    uint64_t errors;
} parser_t;

parser_t init_parser(token_t * token_list, size_t token_count);

#endif // PARSER_H