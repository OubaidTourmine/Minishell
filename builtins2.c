/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:50:08 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/12 21:17:31 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	func_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		printf("%s\n", data->env[i++]);
	}
	g_exit_status = 0;
	return (0);
}

int	check_x_permession(mode_t mode)
{
	return (mode & S_IXUSR);
}

int	func_cd(t_data *data)
{
	setup_old_pwd(data);
	if (!data->commands->args[1])
	{
		if (cd_to_home() != 0)
			return (1);
	}
	else if (!ft_strcmp_ft(data->commands->args[1], "-"))
	{
		if (cd_to_oldpwd() != 0)
			return (1);
	}
	else if (data->commands->args[2])
	{
		ft_putstr_fd(" too many arguments", 2);
		return (1);
	}
	else
	{
		if (cd_to_path(data->commands->args[1]) != 0)
			return (1);
	}
	update_pwd_env(data);
	g_exit_status = 0;
	return (g_exit_status);
}
