/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:51:06 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/14 19:10:10 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	func_echo_norm(t_data *data)
{
	int	i;
	int	n_flag;
	int	j;

	i = 1;
	n_flag = 0;
	while (data->commands->args[i] && data->commands->args[i][0] == '-')
	{
		j = 1;
		while (data->commands->args[i][j] == 'n')
			j++;
		if (data->commands->args[i][j] != '\0')
			break ;
		if (j == 1)
			break ;
		n_flag = 1;
		i++;
	}
	return (n_flag);
}

int	func_echo(t_data *data)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (!data->commands->args[i])
	{
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	n_flag = func_echo_norm(data);
	while (data->commands->args[i])
	{
		ft_putstr_fd(data->commands->args[i], STDOUT_FILENO);
		i++;
		if (data->commands->args[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (n_flag == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	g_exit_status = 0;
	return (0);
}

int	func_exit_error(t_data *data)
{
	if (data->commands->args[1] && data->commands->args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	func_exit(t_data *data)
{
	int	i;

	if (func_exit_error(data) == 1)
		return (EXIT_FAILURE);
	if (data->commands->args[1])
	{
		i = 0;
		while (data->commands->args[1][i])
		{
			if ((data->commands->args[1][i] < '0'
				|| data->commands->args[1][i] > '9')
				&& data->commands->args[1][i] != '+'
				&& data->commands->args[1][i] != '-')
			{
				ft_putstr_fd(" numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		g_exit_status = ft_atoi(data->commands->args[1]);
	}
	else
		g_exit_status = 0;
	free_memory();
	exit(g_exit_status);
}
