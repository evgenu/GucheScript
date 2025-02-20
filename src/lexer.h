#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

typedef struct Token {
    char *value;
    char *type;
    uint64_t pos;
} token_t;

typedef struct FileContent {
    char *content;
    size_t size;
} file_content_t;

typedef struct Lexer {
    file_content_t file_content;
    uint64_t cur;
    uint64_t bol;
    uint64_t line;
} lexer_t;

int open_file ( const char *filename, file_content_t* file_content );
lexer_t init_lexer ( file_content_t src );
void chop_char (lexer_t * lexer );
void trim_left ( lexer_t * lexer);
void drop_line ( lexer_t * lexer );
int is_empty ( lexer_t * lexer );
int is_not_empty ( lexer_t * lexer );
token_t next_token ( lexer_t * lexer);



#endif // LEXER_H