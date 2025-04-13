#include "../minishell.h"

char	*get_var_name(char *line, int *i, t_data *data)
{
	char	*var_name;

	var_name = ft_strdup("", data);
	(*i)++;
	while (line[*i] && is_valid_var_char(line[*i]))
	{
		var_name = add_char_to_str(var_name, line[*i], data);
		(*i)++;
	}
	return (var_name);
}

char	*expand_variable(char *line, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	
	var_name = get_var_name(line, i, data);
	var_value = getenv(var_name);
	if(data->is_filename)
	{
	if(data->redir_var_name)
	ft_free(data->redir_var_name, data);
	data->redir_var_name = var_name;
	}
	else
	ft_free(var_name, data);
	if (!var_value)
		var_value = ft_strdup("", data);
	else
		var_value = ft_strdup(var_value, data);
	return (var_value);
}
