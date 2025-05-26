/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:24:52 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/18 16:15:39 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char **env)
{
	int		i;
	int		j;
	char	*name;

	j = 0;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (!ft_strncmp_ft(name, "PATH", 4))
		{
			ft_free(name);
			return (env[i] + j + 1);
		}
		ft_free(name);
		i++;
	}
	return (NULL);
}

char	*ft_check_command(char *cmd, char *path)
{
	char	**splited_path;
	int		i;
	char	*join_path;
	char	*join_cmd;

	i = 0;
	if (path == NULL)
		return (NULL);
	splited_path = ft_split(path, ':');
	while (splited_path[i])
	{
		join_path = ft_strjoin(splited_path[i], "/");
		join_cmd = ft_strjoin(join_path, cmd);
		if (!access(join_cmd, F_OK | X_OK))
		{
			ft_free(join_path);
			ft_free(splited_path);
			return (join_cmd);
		}
		ft_free(join_cmd);
		ft_free(join_path);
		i++;
	}
	ft_free(splited_path);
	return (NULL);
}

char	**join_cmd_with_path(char **cmd, char **env)
{
	char	*path;
	char	**res;
	char	*new_cmd;

	res = cmd;
	path = get_path(env);
	new_cmd = ft_check_command(cmd[0], path);
	if (new_cmd != NULL)
	{
		if (access(new_cmd, F_OK | X_OK) != 0)
		{
			ft_free(new_cmd);
			res = NULL;
		}
		else
		{
			ft_free(cmd[0]);
			cmd[0] = new_cmd;
		}
	}
	else
		res = NULL;
	return (res);
}

char	**get_command(char *str, char **env)
{
	char	**cmd;
	char	**res;

	if (has_spaces(str))
		return (NULL);
	str = ft_strtrim(str, " ");
	cmd = ft_split(str, ' ');
	res = cmd;
	if (cmd == NULL)
		exit(1);
	if (cmd[0] == NULL)
		res = NULL;
	else if (is_path(cmd[0]))
	{
		if (access(cmd[0], F_OK | X_OK) != 0)
			res = NULL;
	}
	else
		res = join_cmd_with_path(cmd, env);
	ft_free(str);
	if (!res)
		ft_free(cmd);
	return (res);
}

int	handle_pipe(t_data *data)
{
	t_data_pipe	p_data;
	int			cap_ret;
	int			status;

	pipe_data_init(&p_data, data);
	while (p_data.i <= p_data.count)
	{
		cap_ret = routine_proccess(data, &p_data);
		if (cap_ret == EXIT_FAILURE)
		{
			g_exit_status = EXIT_FAILURE;
			return (EXIT_FAILURE);
		}
	}
	close(p_data.p_fd[0]);
	p_data.i = -1;
	p_data.count++;
	while (++(p_data.i) < p_data.count)
		waitpid(p_data.pid[p_data.i], &status, 0);
	g_exit_status = ft_get_exit_status(status);
	return (g_exit_status);
}
