#include "../minishell.h"

int  skip_spaces(char *line, int *i)
{
    int spaces;

    spaces = 0;
    while(ft_isspace(line[*i]))
    {
    (*i)++;
    spaces++;
    }
    return spaces;
}
