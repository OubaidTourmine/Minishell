/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:17:28 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 19:52:45 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_dup(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, 1);
		close(fd_out);
	}
}

size_t	get_how_many_pipe(t_data *data)
{
	size_t		cmd_count;
	t_command	*current;

	cmd_count = 0;
	current = data->commands;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}
	return (cmd_count);
}

void	get_in_ou_fd(t_data *data, t_data_pipe *p_data)
{
	if (p_data->i == 0)
	{
		data->fd_out = p_data->p_fd[1];
		close(p_data->p_fd[0]);
	}
	else if (p_data->i < p_data->count)
	{
		data->fd_in = p_data->stored_fd_in;
		data->fd_out = p_data->p_fd[1];
		close(p_data->p_fd[0]);
	}
	else if (p_data->i == p_data->count)
	{
		data->fd_in = p_data->stored_fd_in;
		if (!p_data->flag)
			data->fd_out = 1;
	}
}

void	pipe_data_init(t_data_pipe *p_data, t_data *data)
{
	p_data->count = get_how_many_pipe(data) - 1;
	p_data->pid = ft_malloc(sizeof(int) * (p_data->count + 1));
	p_data->for_close = -1;
	p_data->i = 0;
	p_data->flag = 0;
}

int	routine_proccess(t_data *data, t_data_pipe *p_data)
{
	if (p_data->i < p_data->count)
		pipe(p_data->p_fd);
	p_data->pid[p_data->i] = fork();
	if (p_data->pid[p_data->i] == 0)
	{
		get_in_ou_fd(data, p_data);
		if (data->commands->redirects)
		{
			p_data->st_fd_out = data->fd_out;
			handle_redirections(data);
			if (p_data->i == p_data->count && p_data->st_fd_out != data->fd_out)
				p_data->flag = 1;
		}
		handle_cmd(data, 1);
	}
	else if (p_data->pid[p_data->i] < 0)
		return (handle_error());
	p_data->stored_fd_in = p_data->p_fd[0];
	if (p_data->for_close != -1 && p_data->for_close != p_data->stored_fd_in)
		close(p_data->for_close);
	p_data->for_close = p_data->stored_fd_in;
	close(p_data->p_fd[1]);
	data->commands = data->commands->next;
	p_data->i++;
	return (EXIT_SUCCESS);
}
