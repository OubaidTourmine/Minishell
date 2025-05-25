/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:17:42 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:07:39 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_old_pwd(t_data *data)
{
	char	*tmp;

	data->old_pwd = getcwd(NULL, 0);
	tmp = data->old_pwd;
	data->old_pwd = ft_strjoin_ft("OLDPWD=", tmp);
	ft_free(tmp);
}

int	cd_to_home(void)
{
	const char	*path;

	path = getenv("HOME");
	if (!path)
	{
		ft_putstr_fd("getenv failed\n", 2);
		g_exit_status = -1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("chdir failed\n", 2);
		g_exit_status = -1;
		return (1);
	}
	return (0);
}

int	cd_to_oldpwd(void)
{
	const char	*path;

	path = getenv("OLDPWD");
	if (!path)
	{
		ft_putstr_fd("getenv failed\n", 2);
		g_exit_status = -1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("chdir failed\n", 2);
		g_exit_status = -1;
		return (1);
	}
	return (0);
}

int	cd_to_path(const char *path)
{
	struct stat	file_stat;

	if (!path)
		return (1);
	if (stat(path, &file_stat) == -1)
	{
		ft_putstr_fd(" No such file or directory", 2);
		g_exit_status = -1;
		return (1);
	}
	if (S_ISDIR(file_stat.st_mode) && check_x_permession(file_stat.st_mode))
	{
		if (chdir(path) == -1)
		{
			ft_putstr_fd("chdir failed\n", 2);
			g_exit_status = -1;
			return (1);
		}
	}
	return (0);
}

void	update_pwd_env(t_data *data)
{
	char	*tmp;
	int		i;

	data->pwd = getcwd(NULL, 0);
	tmp = data->pwd;
	data->pwd = ft_strjoin_ft("PWD=", tmp);
	ft_free(tmp);
	i = ft_get_index_env(data->env, "OLDPWD=");
	data->env[i] = data->old_pwd;
	i = ft_get_index_env(data->env, "PWD=");
	data->env[i] = data->pwd;
}
