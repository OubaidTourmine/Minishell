/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:16:10 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 16:32:55 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection(t_data *data, int flag)
{
	int		file;
	char	**args;
	int		i;
	char	**path;
	char	*cmd_path;
	int		pid;
		char *name;
	int		fd;

	file = 0;
	if (data->commands->redirects->target && flag == 1)
		file = open(data->commands->redirects->target,
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (data->commands->redirects->target && flag == 2)
		file = open(data->commands->redirects->target, O_RDONLY, 0777);
	else if (data->commands->redirects->target && flag == 3)
		file = open(data->commands->redirects->target,
				O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
	{
		name = generate_heredoc_filepath();
		fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0777);
		ft_putstr_fd(data->commands->redirects->hd_content, fd);
		close(fd);
		fd = open(name, O_RDONLY);
	}
	i = 0;
	if (file == -1)
		return (2);
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
			if (flag == 1)
				dup2(file, STDOUT_FILENO);
			else if (flag == 2)
				dup2(file, STDIN_FILENO);
			else if (flag == 3)
				dup2(file, STDOUT_FILENO);
			else if (flag == 4)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			if(flag != 4)
				close(file);
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
	if(flag == 4)
		close(fd);
	else
		close(file);
	return (0);
}

void	handle_redirections(t_data *data)
{
	if (data->commands->redirects->type == OUTPUT)
		redirection(data, 1);
	else if (data->commands->redirects->type == INPUT)
		redirection(data, 2);
	else if (data->commands->redirects->type == APPEND)
		redirection(data, 3);
	else if (data->commands->redirects->type == HEREDOC)
		redirection(data, 4);
}


/*

	create function "handle_cmd"
	create function "handle_redirection"

*/