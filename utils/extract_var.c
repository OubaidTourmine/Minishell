#include "../minishell.h"

char *extract_var(char *var)
{
    char *new_var;
    int i;

    if (!has_append_operator(var))
        return ft_strdup(var);

    i = 0;
    new_var = ft_strdup("");
    while (var[i])
    {
        if (var[i] == '+')
        {
            i++;
            break;
        }
        new_var = add_char_to_str(new_var, var[i++]);
    }
    while(var[i])
    new_var = add_char_to_str(new_var, var[i++]);

    return new_var;

}