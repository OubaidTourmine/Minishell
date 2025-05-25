/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:22:20 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:19:54 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_to_next_command(t_token **token)
{
	while (*token && (*token)->type != PIPE)
		(*token) = (*token)->next;
}

char	*extract_amb_error_msg(char *line)
{
	int		i;
	char	*amb_msg;

	i = 0;
	while (line[i] && !ft_isspace(line[i]) && line[i] != '<' && line[i] != '>'
		&& line[i] != '|')
		i++;
	amb_msg = ft_strjoin_free(ft_strdup("minishell: "), ft_substr(line, 0, i));
	amb_msg = ft_strjoin_free(amb_msg, ft_strdup(": ambiguous redirect\n"));
	return (amb_msg);
}

int	check_access(char *filename, t_type type)
{
	char	*dir_path;

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
		dir_path = get_dir_path(filename);
		if ((access(dir_path, W_OK) == -1) || (access(filename, F_OK) != -1
				&& access(filename, W_OK) == -1))
		{
			ft_putstr_fd("minishell: ", 2);
			return (perror(filename), 1);
		}
	}
	return (0);
}

void	mark_amb(char *line, int start_i, t_token *amb_redirect)
{
	amb_redirect->is_amb_redirect = 1;
	amb_redirect->amb_error_msg = extract_amb_error_msg(line + start_i);
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
