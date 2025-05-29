/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:48:24 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/19 17:02:05 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	func_unset_env(t_data *data, char *var_name, int len_args,
		int arg_index)
{
	int	i;
	int	j;
	int	var_len;

	i = 0;
	i = 0;
	while (data->env[i])
	{
		var_name = data->env[i];
		var_len = ft_strlen_exp(var_name);
		if (ft_strncmp_ft(var_name, data->commands->args[arg_index],
				len_args) == 0 && var_len == len_args)
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

void	func_unset_export(t_data *data, char *var_name, int len_args,
		int arg_index)
{
	int	i;
	int	j;
	int	var_len;

	i = 0;
	i = 0;
	while (data->export[i])
	{
		var_name = data->export[i];
		var_len = ft_strlen_exp(var_name);
		if (ft_strncmp_ft(var_name, data->commands->args[arg_index],
				len_args) == 0 && var_len == len_args)
		{
			ft_free(data->export[i]);
			j = i;
			while (data->export[j])
			{
				data->export[j] = data->export[j + 1];
				j++;
			}
			continue ;
		}
		i++;
	}
}

void	func_unset_loop(t_data *data, int arg_index)
{
	char	*var_name;
	int		len_args;

	var_name = NULL;
	len_args = ft_strlen_ft(data->commands->args[arg_index]);
	func_unset_env(data, var_name, len_args, arg_index);
	func_unset_export(data, var_name, len_args, arg_index);
}

int	func_unset(t_data *data)
{
	int	arg_index;

	if (!data->commands->args[1])
		return (0);
	arg_index = 1;
	while (data->commands->args[arg_index])
	{
		if (!is_valid_identifier(data->commands->args[arg_index]))
		{
			print_export_error(data->commands->args[arg_index]);
			g_exit_status = 1;
		}
		else
		{
			func_unset_loop(data, arg_index);
		}
		arg_index++;
	}
	if (g_exit_status != 1)
		g_exit_status = 0;
	return (g_exit_status);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{	
		if(str[i] == '+' && str[i + 1] == '=')
		return 1;
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
