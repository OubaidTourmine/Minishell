#include "../minishell.h"

void	tokenize_words(t_data *data, char *line, int *i)
{
	t_token	*redir_token;
	int		start_index;

	redir_token = data->last_token;
	start_index = *i;
	add_token_to_list(data, WORD, NULL);
	while (line[*i] && !ft_isspace(line[*i]) && line[*i] != '|'
		&& line[*i] != '<' && line[*i] != '>')
	{
		if (line[*i] == '\'')
			handle_single_quotes(data, line, i);
		else if (line[*i] == '"')
			handle_double_quotes(data, line, i);
		else if (line[*i] == '$' && is_expandable(line[*i + 1])
			&& !data->is_delimiter)
			handle_var_expansion(data, line, i);
		else if (line[*i])
			handle_regular_word(data, line, i);
		// if (data->last_token->has_syntax_error)
		// 	return ;
	}
	if (data->is_filename && is_amb_redirect(redir_token))
		mark_amb(data, line, start_index, redir_token);
	set_flags(data, 0, 0);
}

void	tokenize_redirections(t_data *data, char *line, int *i)
{
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token_to_list(data, HEREDOC, ft_strdup("<<", data));
			(*i) += 2;
			set_flags(data, 1, 0);
			return ;
		}
		else
			add_token_to_list(data, INPUT, ft_strdup("<", data));
	}
	else if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token_to_list(data, APPEND, ft_strdup(">>", data));
			(*i)++;
		}
		else
			add_token_to_list(data, OUTPUT, ft_strdup(">", data));
	}
	(*i)++;
	set_flags(data, 0, 1);
}

void	tokenize_pipes(t_data *data, int *i)
{
	add_token_to_list(data, PIPE, ft_strdup("|", data));
	(*i)++;
	set_flags(data, 0, 0);
}

int	tokenize_input(char *line, t_data *data)
{
	int		i;

	i = 0;
	data->line_pos = &i;
	if (check_leading_pipe(line, &i))
		return (1);
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (line[i] == '|')
			tokenize_pipes(data, &i);
		else if (line[i] == '<' || line[i] == '>')
			tokenize_redirections(data, line, &i);
		else if (line[i])
			tokenize_words(data, line, &i);
		if (data->found_syntax_error)
			return (ft_free(line, data), 0);
	
	}
	
	return (ft_free(line, data), 0);
}
