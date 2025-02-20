#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum
{
    INT,
    FLOAT,
    STRING,
    BOOL,
    CHAR,
} variable_type;

typedef struct Variable
{
    char *name;
    char *value;
    variable_type type;
} variable_t;

typedef struct VariableTable
{
    variable_t *variables;
    uint64_t size;
} variable_table_t;

variable_t create_variable(const char *name, const char *value, variable_type type);
variable_table_t init_variable_table();
void add_variable(variable_table_t *variable_table, variable_t variable);

#endif // SYMBOL_TABLE_H