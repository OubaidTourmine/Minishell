/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:16:10 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/20 16:32:10 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(t_data *data, int flag)
{
	int	file;

	file = 0;
	if (data->commands->redirects->target && flag == 1)
		file = open(data->commands->redirects->target,
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (data->commands->redirects->target && flag == 2)
		file = open(data->commands->redirects->target, O_RDONLY, 0777);
	else if (data->commands->redirects->target && flag == 3)
		file = open(data->commands->redirects->target,
				O_WRONLY | O_CREAT | O_APPEND, 0777);
	return (file);
}

void	handle_redirections(t_data *data)
{
	int	close_in;
	int	close_out;

	close_in = data->fd_in;
	close_out = data->fd_out;
	while (data->commands->redirects)
	{
		if (data->commands->redirects->type == OUTPUT)
			data->fd_out = open_file(data, 1);
		else if (data->commands->redirects->type == INPUT)
			data->fd_in = open_file(data, 2);
		else if (data->commands->redirects->type == APPEND)
			data->fd_out = open_file(data, 3);
		else if (data->commands->redirects->type == HEREDOC)
			data->fd_in = open_file(data, 4);
		if (close_in != 0 && close_in != data->fd_in)
			close(close_in);
		if (close_out != 1 && close_out != data->fd_out)
			close(close_out);
		close_in = data->fd_in;
		close_out = data->fd_out;
		data->commands->redirects = data->commands->redirects->next;
	}
}
