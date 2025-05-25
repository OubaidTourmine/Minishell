/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:13:36 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:07:42 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

void	handle_path_command(t_data *data)
{
	struct stat	file_stat;

	if (stat(data->commands->args[0], &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(data->commands->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(data->commands->args[0], X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(data->commands->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
}

void	execute_command(t_data *data)
{
	char	**s_cmd;

	s_cmd = get_command(data->commands->args[0], data->env);
	if (s_cmd)
	{
		execve(s_cmd[0], data->commands->args, data->env);
		exit(126);
	}
	else if ((data->commands->args[0][0] == '.'
		&& data->commands->args[0][1] == '/')
		|| data->commands->args[0][0] == '/')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(data->commands->args[0], 2);
		ft_putstr_fd(" : No such file or directory\n", 2);
		exit(127);
	}
	else
	{
		ft_putstr_fd(data->commands->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}

void	exec_cmd(t_data *data)
{
	if (data->commands->has_invalid_redirect)
		exit(1);
	else if (!data->commands->args)
		exit(0);
	if (data->commands->args[0][0] == '.' || data->commands->args[0][0] == '/')
	{
		handle_path_command(data);
	}
	execute_command(data);
}

void	handle_cmd_norm(t_data *data, int status)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, handle_child_sigint);
		signal(SIGQUIT, handle_child_sigquit);
		do_dup(data->fd_in, data->fd_out);
		exec_cmd(data);
	}
	else if (pid < 0)
	{
		ft_putstr_fd("fork failed", 2);
		g_exit_status = 1;
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, handle_sigint);
		g_exit_status = ft_get_exit_status(status);
	}
}

int	handle_cmd(t_data *data, int flag)
{
	int	status;

	status = 0;
	if (!builtin_checker(data))
	{
		status = builtin(data);
		g_exit_status = status;
		if (flag)
			exit(status);
		return (status);
	}
	handle_cmd_norm(data, status);
	if (flag)
		exit(g_exit_status);
	return (g_exit_status);
}
