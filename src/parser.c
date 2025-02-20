#include "parser.h"

parser_t init_parser(token_t *token_list, size_t token_count)
{
    return (parser_t){
        .token_list = token_list,
        .errors = 0,
    };
}