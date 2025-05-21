#include "../minishell.h"

void	skip_to_next_command(t_token **token)
{
	while (*token && (*token)->type != PIPE)
		(*token) = (*token)->next;
}

char	*extract_amb_error_msg(t_data *data, char *line)
{
	int		i;
	char	*amb_msg;

	i = 0;
	while (line[i] && !ft_isspace(line[i]) && line[i] != '<' && line[i] != '>'
		&& line[i] != '|')
		i++;
	amb_msg = ft_strjoin_free(ft_strdup("minishell: ", data), ft_substr(line, 0,
				i, data), data);
	amb_msg = ft_strjoin_free(amb_msg, ft_strdup(": ambiguous redirect\n",
				data), data);
	return (amb_msg);
}

int	check_access(char *filename, t_type type)
{
	if (type == INPUT)
	{
		if (access(filename, F_OK) == -1 || access(filename, R_OK))
		{
			ft_putstr_fd("minishell: ", 2);
			return (perror(filename), 1);
		}
	}
	else
	{
		if (access(filename, F_OK) != -1 && access(filename, W_OK) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			return (perror(filename), 1);
		}
	}
	return (0);
}

void	mark_amb(t_data *data, char *line, int start_i, t_token *amb_redirect)
{
	amb_redirect->is_amb_redirect = 1;
	amb_redirect->amb_error_msg = extract_amb_error_msg(data, line + start_i);
	// while (line[*data->line_pos] && line[*data->line_pos] != '|')
	// 	(*data->line_pos)++;
}

void	report_redirect_errors(t_data *data)
{
	t_token	*token;

	token = data->tokens;
	while (token)
	{
		if (is_redirection(token->type))
		{
			if (token->is_amb_redirect)
				ft_putstr_fd(token->amb_error_msg, 2);
			else if (is_redirection(token->type))
			{
				if (check_access(token->next->content, token->type))
				token->has_invalid_file = 1;
			}
		}
		if (token->has_invalid_file || token->is_amb_redirect)
			skip_to_next_command(&token);
		else
			token = token->next;
	}
}
