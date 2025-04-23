/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:46:49 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 16:36:19 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_envi(t_data *data, char *path)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (strncmp(path, data->env[i], ft_strlen_ft(path)) == 0)
		{
			return (ft_split(data->env[i] + 5, ':'));
		}
		i++;
	}
	return (0);
}

char	*cmd_found(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (access(path[i], X_OK) == 0)
			return (path[i]);
		i++;
	}
	return (NULL);
}

int	do_child(t_data *data, char *cmd_path, int flag)
{
	int			pid;
	char		**args;
	int			cmd_count;
	t_command	*current;
	int			i;
	pid_t		*pids;
	char		**path;
	int			j;
	char		*current_cmd_path;
	t_redirect	*redir;
	int			file;
	int			k;

	if (flag == 0)
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
	else // Pipeline execution
	{
		// Count the number of commands
		cmd_count = 0;
		current = data->commands;
		while (current)
		{
			cmd_count++;
			current = current->next;
		}
		if (cmd_count < 2)
			return (0);
		// Create pipes (need cmd_count - 1 pipes)
		int(*pipes)[2] = malloc(sizeof(int[2]) * (cmd_count - 1));
		if (!pipes)
		{
			perror("malloc failed");
			return (-1);
		}
		// Create all pipes
		i = 0;
		while (i < cmd_count - 1)
		{
			if (pipe(pipes[i]) == -1)
			{
				perror("pipe");
				free(pipes);
				return (-1);
			}
			i++;
		}
		// Array to store child PIDs
		pids = malloc(sizeof(pid_t) * cmd_count);
		if (!pids)
		{
			free(pipes);
			perror("malloc failed");
			return (-1);
		}
		current = data->commands;
		i = 0;
		while (i < cmd_count)
		{
			// Get command path for current command
			path = get_envi(data, "PATH");
			if (!path)
				return (-1);
			j = 0;
			while (path[j])
			{
				path[j] = ft_strjoin_ft(path[j], "/");
				j++;
			}
			j = 0;
			while (path[j])
			{
				path[j] = ft_strjoin_ft(path[j], current->args[0]);
				j++;
			}
			current_cmd_path = cmd_found(path);
			pids[i] = fork();
			if (pids[i] == 0) // Child process
			{
				// Set up pipe connections
				if (i > 0) // Not first command - read from previous pipe
				{
					dup2(pipes[i - 1][0], STDIN_FILENO);
				}
				if (i < cmd_count - 1) // Not last command - write to next pipe
				{
					dup2(pipes[i][1], STDOUT_FILENO);
				}
				// Handle redirections for this specific command
				if (current->redirects)
				{
					redir = current->redirects;
					while (redir)
					{
						if (redir->type == OUTPUT)
						{
							file = open(redir->target,
									O_WRONLY | O_CREAT | O_TRUNC, 0644);
							if (file != -1)
							{
								dup2(file, STDOUT_FILENO);
								close(file);
							}
						}
						else if (redir->type == INPUT)
						{
							file = open(redir->target, O_RDONLY);
							if (file != -1)
							{
								dup2(file, STDIN_FILENO);
								close(file);
							}
						}
						else if (redir->type == APPEND)
						{
							file = open(redir->target,
									O_WRONLY | O_CREAT | O_APPEND, 0644);
							if (file != -1)
							{
								dup2(file, STDOUT_FILENO);
								close(file);
							}
						}
						redir = redir->next;
					}
				}
				// Close all pipe file descriptors
				k = 0;
				while (k < cmd_count - 1)
				{
					close(pipes[k][0]);
					close(pipes[k][1]);
					k++;
				}
				// Prepare arguments
				if (current->args[1])
				{
					args = malloc(sizeof(char *) * 3);
					if (!args)
					{
						perror("malloc failed");
						exit(EXIT_FAILURE);
					}
					args[0] = current->args[0];
					args[1] = current->args[1];
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
					args[0] = current->args[0];
					args[1] = NULL;
				}
				if (current_cmd_path == NULL)
				{
					printf("%s: command not found\n", current->args[0]);
					exit(127);
				}
				execve(current_cmd_path, args, data->env);
				perror("execve");
				free(args);
				exit(EXIT_FAILURE);
			}
			else if (pids[i] < 0)
			{
				perror("fork");
				free(pipes);
				free(pids);
				return (-1);
			}
			current = current->next;
			i++;
		}
		// Parent: close all pipe file descriptors
		i = 0;
		while (i < cmd_count - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
		// Wait for all children
		i = 0;
		while (i < cmd_count)
		{
			wait(NULL);
			i++;
		}
		free(pipes);
		free(pids);
	}
	return (0);
}

int	execution(t_data *data, int flag)
{
	int		i;
	char	**path;
	char	*cmd_path;

	if (data->commands->next)
		flag = 1; // Pipeline
	else
		flag = 0;  // Single command
	if (flag == 0) // Single command
	{
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
			do_child(data, cmd_path, flag);
		// Free the path array
		i = 0;
		while (path[i])
			free(path[i++]);
		free(path);
	}
	else // Pipeline - let do_child handle finding paths for each command
	{
		do_child(data, NULL, flag);
		// Pass NULL since we'll find paths individually
	}
	return (0);
}
