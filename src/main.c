#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "variable_table.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: no input files\n");
        exit(1);
    }
    file_content_t file_content;
    int err = open_file(argv[1], &file_content);
    if (err)
    {
        fprintf(stderr, "Error: could not open file %s with error: %d\n", argv[1], err);
        exit(1);
    }

    lexer_t lexer = init_lexer(file_content);

    for (size_t i = 0; i < lexer.file_content.size; i++)
    {
        printf("%c", lexer.file_content.content[i]);
    }

    printf("\n\n");

    token_t *tokens = NULL;
    size_t token_count = 0;

    while (1)
    {
        token_t token = next_token(&lexer);
        if (token.value == NULL)
            break;

        token_count++;
        tokens = realloc(tokens, token_count * sizeof(token_t));
        if (tokens == NULL)
        {
            fprintf(stderr, "Error: memory allocation failed\n");
            exit(1);
        }

        tokens[token_count - 1] = token;
    }

    for (size_t i = 0; i < token_count; i++)
    {
        printf("Token: { Value: %s, Type: %s }\n", tokens[i].value, tokens[i].type);
    }

    parser_t parser = init_parser(tokens, token_count);

    free(tokens);
}
