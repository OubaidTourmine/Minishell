#include "../minishell.h"

int	check_access(char *filename, int mode)
{
	if (access(filename, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		return (perror(filename), 1);
	}
	if (access(filename, mode) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		return (perror(filename), 1);
	}
	return (0);
}

void	mark_amb_redirect(t_data *data, char *line, t_token *amb_redirect)
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
	amb_redirect->is_amb_redirect = 1;
	data->stop_tokenization = 1;
	data->amb_msg = amb_msg;
}

int	check_redirect_errors(t_data *data)
{
	t_token	*token;

	token = data->tokens;
	while (token)
	{
		if (is_redirection(token->type))
		{
			if (token->is_amb_redirect)
				return (ft_putstr_fd(data->amb_msg, 2), free_memory(data), 1);
			else if (token->type == INPUT)
			{
				if (check_access(token->next->content, R_OK))
					return (free_memory(data), 1);
			}
			else if (token->type == OUTPUT || token->type == APPEND)
			{
				if (check_access(token->next->content, W_OK))
					return (free_memory(data), 1);
			}
		}
		token = token->next;
	}
	return (0);
}
