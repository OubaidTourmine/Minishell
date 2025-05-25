/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:59:38 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:02:59 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_fork_commands(t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (data->commands->redirects)
			handle_redirections(data);
		do_dup(data->fd_in, data->fd_out);
		if (ft_strcmp_ft(data->commands->args[0], "echo") == 0)
			func_echo(data);
		else if (ft_strcmp_ft(data->commands->args[0], "pwd") == 0)
			func_pwd();
		else if (ft_strcmp_ft(data->commands->args[0], "env") == 0)
			func_env(data);
		exit(0);
	}
	else if (pid < 0)
		return (ft_putstr_fd("fork failed", 2), 1);
	waitpid(pid, &status, 0);
	g_exit_status = ft_get_exit_status(status);
	return (g_exit_status);
}

int	builtin(t_data *data)
{
	if (data->commands->has_invalid_redirect)
		exit(1);
	if (ft_strcmp_ft(data->commands->args[0], "echo") == 0
		|| ft_strcmp_ft(data->commands->args[0], "pwd") == 0
		|| ft_strcmp_ft(data->commands->args[0], "env") == 0)
	{
		return (builtin_fork_commands(data));
	}
	else if (ft_strcmp_ft(data->commands->args[0], "export") == 0)
		g_exit_status = func_export(data);
	else if (ft_strcmp_ft(data->commands->args[0], "unset") == 0)
		g_exit_status = func_unset(data);
	else if (ft_strcmp_ft(data->commands->args[0], "exit") == 0)
		g_exit_status = func_exit(data);
	else if (ft_strcmp_ft(data->commands->args[0], "cd") == 0)
		g_exit_status = func_cd(data);
	return (g_exit_status);
}

int	builtin_checker(t_data *data)
{
	if (!data || !data->commands || !data->commands->args
		|| !data->commands->args[0])
	{
		return (1);
	}
	if (ft_strcmp_ft(data->commands->args[0], "echo") == 0
		|| ft_strcmp_ft(data->commands->args[0], "cd") == 0
		|| ft_strcmp_ft(data->commands->args[0], "pwd") == 0
		|| ft_strcmp_ft(data->commands->args[0], "env") == 0
		|| ft_strcmp_ft(data->commands->args[0], "export") == 0
		|| ft_strcmp_ft(data->commands->args[0], "unset") == 0
		|| ft_strcmp_ft(data->commands->args[0], "exit") == 0)
		return (0);
	return (1);
}

int	get_env(char **env, t_data *data)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
		i++;
	data->env = ft_malloc((i) * sizeof(char *) + 8);
	if (!data->env)
		return (1);
	i = 0;
	while (env[i] != NULL)
	{
		data->env[i] = ft_strdup_ft(env[i]);
		i++;
	}
	data->env[i] = NULL;
	return (0);
}

int	ft_get_index_env(char **env, char *target)
{
	int	ret;
	int	target_len;

	ret = 0;
	target_len = ft_strlen_exp(target);
	while (env[ret])
	{
		if (ft_strncmp_ft(env[ret], target, target_len) == 0
			&& (env[ret][target_len] == '=' || env[ret][target_len] == '\0'))
			return (ret);
		ret++;
	}
	return (-1);
}
