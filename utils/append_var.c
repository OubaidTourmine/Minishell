#include "../minishell.h"

char *append_var(char *existing_var, char *new_var)
{

    int i;

    i = 0;
    while (new_var[i] && new_var[i] != '+')
        i++;
    i += 2;
    if (!new_var[i])
        return ft_strdup(existing_var);
    else
        return (ft_strjoin(existing_var, new_var + i));
}