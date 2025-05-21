#include "../minishell.h"

void	handle_regular_word(t_data *data, char *line, int *i)
{
	char	*word;

	word = ft_strdup("", data);
	while (line[*i] && line[*i] != '\'' && line[*i] != '"' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != '|' && !ft_isspace(line[*i]))
	{
		if (line[*i] == '$' && is_expandable(line[*i + 1])
			&& !data->is_delimiter)
			break ;
		word = add_char_to_str(word, line[*i], data);
		(*i)++;
	}
	append_to_last_token(data, word, 0);
}

void	handle_single_quotes(t_data *data, char *line, int *i)
{
	char	*word;

	(*i)++;
	word = ft_strdup("", data);
	while (line[*i] && line[*i] != '\'')
	{
		word = add_char_to_str(word, line[*i], data);
		(*i)++;
	}
	if (!line[*i])
	{
		ft_putstr_fd("minishell: unexpected EOF ", 2);
		ft_putstr_fd("while looking for matching `''\n", 2);
		append_to_last_token(data, word, 1);
		data->last_token->has_syntax_error = 1;
		return ;
	}
	(*i)++;
	append_to_last_token(data, word, 1);
}

void	handle_double_quotes(t_data *data, char *line, int *i)
{
	char	*word;

	(*i)++;
	word = ft_strdup("", data);
	while (line[*i] && line[*i] != '"')
	{
		if (line[*i] == '$' && is_expandable(line[*i + 1])
			&& !data->is_delimiter)
			word = ft_strjoin_free(word, expand(line, i, data), data);
		else if (line[*i])
		{
			word = add_char_to_str(word, line[*i], data);
			(*i)++;
		}
	}
	if (!line[*i])
	{
		ft_putstr_fd("minishell: unexpected EOF ", 2);
		ft_putstr_fd("while looking for matching `\"'\n", 2);
		append_to_last_token(data, word, 1);
		data->last_token->has_syntax_error = 1;
		return ;
	}
	(*i)++;
	append_to_last_token(data, word, 1);
}

void	handle_var_expansion(t_data *data, char *line, int *i)
{
	int		j;
	char	*var_value;
	char	*word;

	j = 0;
	word = NULL;
	var_value = expand(line, i, data);
	check_lead_space(data, var_value);
	skip_spaces(var_value, &j);
	while (var_value[j])
	{
		if (ft_isspace(var_value[j]))
			handle_spaces_in_var(data, var_value, &j);
		else
		{
			while (var_value[j] && !ft_isspace(var_value[j]))
				word = add_char_to_str(word, var_value[j++], data);
			append_to_last_token(data, word, 0);
			word = ft_strdup("", data);
		}
	}
	check_trail_space(data, var_value, line, i);
	ft_free(var_value, data);
}
