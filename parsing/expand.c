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

char	*expand_exit_status(int *i, t_data *data)
{
	char	*exit_status;

	exit_status = ft_itoa(data->exit_status, data);
	(*i) += 2;
	return (exit_status);
}

char	*expand(char *line, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;

	if (line[*i + 1] == '?')
		return (expand_exit_status(i, data));
	var_name = get_var_name(line, i, data);
	var_value = getenv(var_name);
	ft_free(var_name, data);
	if (!var_value)
		var_value = ft_strdup("", data);
	else
		var_value = ft_strdup(var_value, data);
	return (var_value);
}
