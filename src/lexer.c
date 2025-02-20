#include "lexer.h"

int open_file(const char *filename, file_content_t *file_content)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return ERR_OPEN_FILE;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *src = (char *)malloc(file_size + 1);
    if (!src)
    {
        return ERR_ALLOC_MEM;
    }

    fread(src, 1, file_size, file);
    src[file_size] = '\0';

    fclose(file);
    file_content->content = src;
    file_content->size = file_size;
    return ERR_NO;
}

void chop_char(lexer_t *lexer)
{
    if (lexer->cur < lexer->file_content.size)
    {
        char c = lexer->file_content.content[lexer->cur];
        lexer->cur++;
        if (c == '\n')
        {
            lexer->line++;
            lexer->bol = lexer->cur;
        }
    }
}

void trim_left(lexer_t *lexer)
{
    while (is_not_empty(lexer))
    {
        char c = lexer->file_content.content[lexer->cur];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            chop_char(lexer);
        else
            break;
    }
}

void drop_line(lexer_t *lexer)
{
    while (is_not_empty(lexer) && lexer->file_content.content[lexer->cur] != '\n')
        chop_char(lexer);
    if (is_not_empty(lexer))
        chop_char(lexer);
}

int is_empty(lexer_t *lexer)
{
    return !is_not_empty(lexer);
}

int is_not_empty(lexer_t *lexer)
{
    return lexer->cur < lexer->file_content.size;
}

token_t next_token(lexer_t *lexer)
{
    trim_left(lexer);

    while (is_not_empty(lexer) && lexer->file_content.content[lexer->cur] == '#')
    {
        drop_line(lexer);
        trim_left(lexer);
    }

    if (is_empty(lexer))
    {
        token_t token = {
            .value = NULL,
            .type = "EOF",
            .pos = lexer->cur,
        };
        return token;
    }
    else
    {
        char c = lexer->file_content.content[lexer->cur];
        token_t token;

        if (isalpha(c))
        {
            size_t start = lexer->cur;
            while (is_not_empty(lexer) && isalnum(lexer->file_content.content[lexer->cur]))
            {
                chop_char(lexer);
            }
            size_t length = lexer->cur - start;
            char *value = (char *)malloc(length + 1);
            strncpy(value, &lexer->file_content.content[start], length);
            value[length] = '\0';

            token.value = value;
            token.type = "IDENTIFIER";
            token.pos = start;
        }
        else if (isdigit(c))
        {
            size_t start = lexer->cur;
            while (is_not_empty(lexer) && isdigit(lexer->file_content.content[lexer->cur]))
            {
                chop_char(lexer);
            }
            size_t length = lexer->cur - start;
            char *value = (char *)malloc(length + 1);
            strncpy(value, &lexer->file_content.content[start], length);
            value[length] = '\0';

            token.value = value;
            token.type = "NUMBER";
            token.pos = start;
        }
        else
        {
            chop_char(lexer);
            token.value = (char *)malloc(2);
            token.value[0] = c;
            token.value[1] = '\0';
            token.type = "SYMBOL";
            token.pos = lexer->cur - 1;
        }

        return token;
    }
}

lexer_t init_lexer(file_content_t src)
{
    return (lexer_t){
        .file_content = src,
        .cur = 0,
        .bol = 0,
        .line = 1,
    };
}
