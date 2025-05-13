#include "../minishell.h"

int check_amb_redirect(t_data *data)
{
    if(data->amb_redirect)
    {
    write(2, "minishell: $", 12);
    write(2, data->redir_var_name, ft_strlen(data->redir_var_name));
    write(2, ": ambiguous redirect\n", 21);
    free_memory(data);
    return 1;
    }
    else if(data->redir_var_name)
    ft_free(data->redir_var_name, data);
    return 0;
}