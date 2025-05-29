#include "../minishell.h"

int has_append_operator(char *var)
{
    int i;

    i = 0;
    while(var[i])
    {
        if(var[i] == '+' && var[i + 1] == '=')
        return 1;
        if(var[i] == '=')
        break;
        i++;
    }
    return 0;
}