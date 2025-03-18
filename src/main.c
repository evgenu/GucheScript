#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "syntax.h"

int main(int argc, char *argv[])
{

    setvbuf(stdout, NULL, _IOLBF, 0);

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
        printf("Token: { Value: %s, Type: %s }\n", tokens[i].value, tokens[i].type == TOKEN_INT ? "INT" : 
                                                    tokens[i].type == TOKEN_CHAR ? "CHAR" : 
                                                    tokens[i].type == TOKEN_NAME ? "NAME" :
                                                    tokens[i].type == TOKEN_NUMBER ? "NUMBER" : 
                                                    tokens[i].type == TOKEN_OPAREN ? "OPAREN" :
                                                    tokens[i].type == TOKEN_CPAREN ? "CPAREN" :
                                                    tokens[i].type == TOKEN_OCURLY ? "OCURLY" :
                                                    tokens[i].type == TOKEN_CCURLY ? "CCURLY" :
                                                    tokens[i].type == TOKEN_ASSIGN ? "ASSIGN" :
                                                    tokens[i].type == TOKEN_PLUS ? "PLUS" :
                                                    tokens[i].type == TOKEN_MINUS ? "MINUS" :
                                                    tokens[i].type == TOKEN_MUL ? "MUL" :
                                                    tokens[i].type == TOKEN_DIV ? "DIV" :
                                                    tokens[i].type == TOKEN_GREATER ? "GREATER" :
                                                    tokens[i].type == TOKEN_LESS ? "LESS" :
                                                    tokens[i].type == TOKEN_EQUAL ? "EQUAL" :
                                                    tokens[i].type == TOKEN_COMMA ? "COMMA" :
                                                    tokens[i].type == TOKEN_EOF ? "EOF" :
                                                    tokens[i].type == TOKEN_IF ? "IF" :
                                                    tokens[i].type == TOKEN_FUNC ? "FUNC" :
                                                    "UNKNOWN");
    }

    printf("\n");

    parser_t parser = {tokens, 0};
    ast_node_t *ast = parse_function(&parser);

    if (ast == NULL)
    {
        fprintf(stderr, "Error: parsing failed\n");
        exit(1);
    }

    print_ast(ast, 0);
    

    free(tokens);
}
