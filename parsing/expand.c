#include "../minishell.h"

char	*get_var_name(char *line, int *i)
{
	char	*var_name;

	var_name = ft_strdup("");
	(*i)++;
	while (line[*i] && is_valid_var_char(line[*i]))
	{
		var_name = add_char_to_str(var_name, line[*i]);
		(*i)++;
	}
	return (var_name);
}

char	*expand_exit_status(int *i, t_data *data)
{
	(void)data;
	char	*exit_status;

	exit_status = ft_itoa(g_exit_status);
	(*i) += 2;
	return (exit_status);
}

char	*expand(char *line, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;

	if (line[*i + 1] == '?')
		return (expand_exit_status(i, data));
	var_name = get_var_name(line, i);
	var_value = getenv(var_name);
	ft_free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(var_value);
	return (var_value);
}
