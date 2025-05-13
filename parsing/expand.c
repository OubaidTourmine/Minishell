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

char	*get_env_value(t_data *data, char *var_name)
{
	int		index;
	char	*env_entry;
	char	*value;
	int		name_len;

	index = ft_get_index_env(data->env, var_name);
	if (index == -1)
		return (NULL);
	env_entry = data->env[index];
	name_len = ft_strlen_exp(var_name);
	if (env_entry[name_len] == '=')
		value = env_entry + name_len + 1;
	else
		value = "";
	return (ft_strdup(value));
}

char	*expand(char *line, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;

	if (line[*i + 1] == '?')
		return (expand_exit_status(i, data));
	var_name = get_var_name(line, i);
	var_value = get_env_value(data, var_name);
	ft_free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	return (var_value);
}
