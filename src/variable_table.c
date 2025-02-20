#include "variable_table.h"

variable_t create_variable(const char *name, const char *value, variable_type type)
{
    return (variable_t){
        .name = strdup(name),
        .value = strdup(value),
        .type = type,
    };
}

variable_table_t init_variable_table()
{
    return (variable_table_t){
        .variables = (variable_t *)malloc(0),
        .size = 0,
    };
}

void add_variable(variable_table_t *variable_table, variable_t variable)
{
    variable_table->size++;
    variable_table->variables = (variable_t *)realloc(variable_table->variables, variable_table->size * sizeof(variable_t));
    variable_table->variables[variable_table->size - 1] = variable;
}
