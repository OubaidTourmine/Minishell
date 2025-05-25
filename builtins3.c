/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:48:24 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 10:48:22 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	func_pwd(void)
{
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("getcwd failed ", 2);
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	g_exit_status = 0;
	return (0);
}

void	func_unset_loop(int len_args, t_data *data)
{
	char	*var_name;
	int		var_len;
	int		i;
	int		j;

	i = 0;
	while (data->env[i])
	{
		var_name = data->env[i];
		var_len = ft_strlen_exp(var_name);
		if (ft_strncmp_ft(var_name, data->commands->args[1], len_args) == 0
			&& var_len == len_args)
		{
			ft_free(data->env[i]);
			j = i;
			while (data->env[j])
			{
				data->env[j] = data->env[j + 1];
				j++;
			}
			continue ;
		}
		i++;
	}
}

int	func_unset(t_data *data)
{
	int	len_args;

	if (!data->commands->args[1])
		return (0);
	len_args = ft_strlen_ft(data->commands->args[1]);
	func_unset_loop(len_args, data);
	g_exit_status = 0;
	return (0);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_export_error(char *arg)
{
	ft_putstr_fd("bash: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}
