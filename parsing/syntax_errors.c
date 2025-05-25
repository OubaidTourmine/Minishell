/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:22:06 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/13 16:50:04 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_redirection_error(t_type type)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if (type == PIPE)
		ft_putstr_fd("`|'\n", 2);
	else if (type == INPUT)
		ft_putstr_fd("`<'\n", 2);
	else if (type == OUTPUT)
		ft_putstr_fd("`>'\n", 2);
	else if (type == HEREDOC)
		ft_putstr_fd("`<<'\n", 2);
	else if (type == APPEND)
		ft_putstr_fd("`>>'\n", 2);
}

int	check_leading_pipe(char *line, int *i)
{
	skip_spaces(line, i);
	if (line[*i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_consecutive_operators(t_token *token)
{
	if (is_redirection(token->type) && token->next && !token->is_amb_redirect
		&& token->next->type != WORD)
		return (print_redirection_error(token->next->type), 1);
	else if (token->type == PIPE && token->next && token->next->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_syntax_errors(t_token *token)
{
	if (token->is_amb_redirect)
		return (0);
	else if (token->has_syntax_error)
		return (g_exit_status = 1, 1);
	else if (!token->next)
	{
		if (is_redirection(token->type))
		{
			ft_putstr_fd("minishell: syntax error near ", 2);
			ft_putstr_fd("unexpected token `newline'\n", 2);
			return (g_exit_status = 1, 1);
		}
		else if (token->type == PIPE)
		{
			ft_putstr_fd("minishell: syntax error: ", 2);
			ft_putstr_fd("unexpected end of file\n", 2);
			return (g_exit_status = 1, 1);
		}
	}
	return (0);
}
