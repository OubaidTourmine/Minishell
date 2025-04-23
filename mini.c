/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:13:36 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 15:08:18 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_x_permession(mode_t mode)
{
	return (mode & S_IXUSR);
}

int	func_pwd(t_data *data)
{
	if (!data->env)
		printf("%s\n", data->pwd);
	else
	{
		data->pwd = getenv("PWD");
		if (!data->pwd)
		{
			perror("getenv failed\n");
			return (1);
		}
		printf("%s\n", data->pwd);
	}
	return (0);
}

int	func_cd(t_data *data)
{
	const char	*path;
	struct stat	file_stat;

	if (!data->commands->args)
	{
		path = getenv("HOME");
		if (!path)
		{
			perror("getenv failed\n");
			return (1);
		}
		if (chdir(path) == -1)
		{
			perror("chdir failed\n");
			return (1);
		}
		return (0);
	}
	else if (!ft_strcmp_ft(data->commands->args[1], "-"))
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			perror("getenv failed\n");
			return (1);
		}
		if (chdir(path) == -1)
		{
			perror("chdir failed\n");
			return (1);
		}
		return (0);
	}
	else
	{
		path = data->commands->args[1];
		if (!path)
			return (1);
		if (stat(path, &file_stat) == -1)
		{
			perror("stat failed\n");
			return (1);
		}
		if (S_ISDIR(file_stat.st_mode) && check_x_permession(file_stat.st_mode))
		{
			if (chdir(path) == -1)
			{
				perror("chdir failed\n");
				return (1);
			}
			return (0);
		}
	}
	return (1);
}

int	func_echo(t_data *data)
{
	printf("%s\n", data->commands->args[1]);
	return (0);
}

int	func_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		// if (!data->env[i  +1])
		// {
		//     break ;
		// }
		printf("%s\n", data->env[i++]);
	}
	return (0);
}

int	func_exit(t_data *data)
{
	(void)data;
	if (data->env)
		free_array(data->env);
	if (data->export)
		free_array(data->export);
	free(data);
	exit(0);
}

size_t	ft_strlen_exp(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		return (0);
	}
	while (s[i] && s[i] != '=')
	{
		i++;
	}
	return (i);
}

int	func_unset(t_data *data)
{
	int		i;
	int		var_len;
	int		len_args;
	char	*var_name;
	int		j;

	i = 0;
	len_args = ft_strlen_ft(data->commands->args[0]);
	while (data->env[i])
	{
		var_name = data->env[i];
		var_len = ft_strlen_exp(var_name);
		if (strncmp(var_name, data->commands->args[1], len_args) == 0
			&& var_len == len_args)
		{
			// Free both arrays' entries at this position
			free(data->env[i]);
			free(data->export[i]);
			// Shift all subsequent elements
			j = i;
			while (data->env[j + 1])
			{
				data->env[j] = data->env[j + 1];
				data->export[j] = data->export[j + 1];
				j++;
			}
			// Null-terminate both arrays at the new end
			data->env[j] = NULL;
			data->export[j] = NULL;
			return (0);
		}
		i++;
	}
	return (0);
}

int	func_export(t_data *data)
{
	int	i;

	i = 0;
	get_export(data);
	if (!data->commands->args[1])
	{
		while (data->export[i + 1])
			printf("%s\n", data->export[i++]);
	}
	else
	{
		while (data->env[i])
			i++;
		data->env[i] = ft_strdup_ft(data->commands->args[1]);
		// data->env[i] = ft_strjoin_ft("declare -x ", data->commands->args);
		data->env[i + 1] = NULL;
		i = 0;
		// while (data->export[i])
		// {
		//     // if (!data->export[i  + 2])
		//     // {
		//     //     break ;
		//     // }
		//     // printf("%s\n", data->export[i++]);
		// }
	}
	return (0);
}

int	builtin(t_data *data)
{
	if (ft_strcmp_ft(data->commands->args[0], "echo") == 0)
		func_echo(data);
	else if (ft_strcmp_ft(data->commands->args[0], "cd") == 0)
		func_cd(data);
	else if (ft_strcmp_ft(data->commands->args[0], "pwd") == 0)
		func_pwd(data);
	else if (ft_strcmp_ft(data->commands->args[0], "env") == 0)
		func_env(data);
	else if (ft_strcmp_ft(data->commands->args[0], "export") == 0)
		func_export(data);
	else if (ft_strcmp_ft(data->commands->args[0], "unset") == 0)
		func_unset(data);
	else if (ft_strcmp_ft(data->commands->args[0], "exit") == 0)
		func_exit(data);
	return (0);
}

int	get_env(char **env, t_data *data)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
		i++;
	data->env = malloc((i) * sizeof(char *) + 8);
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

// int execute_pipeline_with_redirections(t_data *data)
// {
// 	// For each command in pipeline:
// 	// Child process:
// 	//   1. Set up pipe connections (stdin from previous, stdout to next)
// 	//   2. Check current->redirects
// 	//   3. Apply redirections (override pipe connections)
// 	//   4. Close unused pipe fds
// 	//   5. execve()
// }

int	execution_pars(t_data *data, char **env)
{
	t_command	*current;
	int			has_pipeline;
	int			has_redirects;

	data->env = NULL;
	data->export = NULL;
	get_env(env, data);
	get_export(data);
	// Check if pipeline has any redirects
	current = data->commands;
	has_pipeline = (data->commands->next != NULL);
	has_redirects = 0;
	while (current)
	{
		if (current->redirects)
		{
			has_redirects = 1;
			break ;
		}
		current = current->next;
	}
	if (has_pipeline && has_redirects)
	{
		execution(data, 1);
	}
	if (data->commands && data->commands->args && data->commands->args[0]
		&& !(data->commands->next) && !(data->commands->redirects)
		&& (ft_strcmp_ft(data->commands->args[0], "echo") == 0
			|| ft_strcmp_ft(data->commands->args[0], "cd") == 0
			|| ft_strcmp_ft(data->commands->args[0], "pwd") == 0
			|| ft_strcmp_ft(data->commands->args[0], "env") == 0
			|| ft_strcmp_ft(data->commands->args[0], "export") == 0
			|| ft_strcmp_ft(data->commands->args[0], "unset") == 0
			|| ft_strcmp_ft(data->commands->args[0], "exit") == 0))
	{
		builtin(data);
	}
	else if (data->commands && data->commands->args && data->commands->args[0]
		&& !(data->commands->redirects))
		execution(data, 0);
	else if (data->commands->redirects)
		handle_redirections(data);
	return (0);
}
