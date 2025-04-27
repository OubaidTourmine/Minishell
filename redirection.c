/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:16:10 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 19:35:33 by outourmi         ###   ########.fr       */
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

int	redirection(t_data *data)
{
	char	**args;
	int		i;
	char	**path;
	char	*cmd_path;
	int		pid;

	i = 0;
	path = get_envi(data, "PATH");
	if (!path)
		return (-1);
	while (path[i])
	{
		path[i] = ft_strjoin_ft(path[i], "/");
		i++;
	}
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin_ft(path[i], data->commands->args[0]);
		i++;
	}
	cmd_path = cmd_found(path);
	if (cmd_path == NULL)
		printf("%s: command not found\n", data->commands->args[0]);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (data->commands->args[1])
			{
				args = malloc(sizeof(char *) * 3);
				if (!args)
				{
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}
				args[0] = data->commands->args[0];
				args[1] = data->commands->args[1];
				args[2] = NULL;
			}
			else
			{
				args = malloc(sizeof(char *) * 2);
				if (!args)
				{
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}
				args[0] = data->commands->args[0];
				args[1] = NULL;
			}
			do_dup(data->fd_in, data->fd_out);
			execve(cmd_path, args, data->env);
			perror("execve");
			free(args);
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
			wait(NULL);
		else
			perror("fork");
	}
	return (0);
}

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
	redirection(data);
}

/*

	create function "handle_cmd"
	create function "handle_redirection"

*/
