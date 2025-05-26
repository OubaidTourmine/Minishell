/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:04:22 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/19 16:23:07 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_vars(t_data *data)
{
	int		i;
	char	**formatted_export;

	formatted_export = get_formatted_export(data);
	i = 0;
	while (formatted_export[i])
	{
		ft_putstr_fd(formatted_export[i], data->fd_out);
		ft_putstr_fd("\n", data->fd_out);
		i++;
	}
	i = 0;
	while (formatted_export[i])
		ft_free(formatted_export[i++]);
	ft_free(formatted_export);
}

void	update_env_var_norm(t_data *data, char *arg, int existing_index)
{
	int	i;

	if (existing_index != -1)
	{
		ft_free(data->export[existing_index]);
		data->export[existing_index] = ft_strdup_ft(arg);
	}
	else
	{
		i = 0;
		while (data->export[i])
			i++;
		data->export[i] = ft_strdup_ft(arg);
		data->export[i + 1] = NULL;
	}
}

void	update_env_var(t_data *data, char *arg)
{
	int	i;
	int	existing_index;

	existing_index = ft_get_index_env(data->env, arg);
	if (existing_index != -1)
	{
		ft_free(data->env[existing_index]);
		data->env[existing_index] = ft_strdup_ft(arg);
	}
	else
	{
		data->env = resize_env_array(data->env);
		if (!data->env)
			return ;
		i = 0;
		while (data->env[i])
			i++;
		data->env[i] = ft_strdup_ft(arg);
		data->env[i + 1] = NULL;
	}
	existing_index = ft_get_index_env(data->export, arg);
	update_env_var_norm(data, arg, existing_index);
}

void	add_env_var_no_value(t_data *data, char *arg)
{
	int	i;
	int	existing_index;

	existing_index = ft_get_index_env(data->export, arg);
	if (existing_index == -1)
	{
		i = 0;
		while (data->export[i])
			i++;
		data->export[i] = ft_strdup_ft(arg);
		data->export[i + 1] = NULL;
	}
}

int	process_export_args(t_data *data)
{
	int	j;

	j = 1;
	while (data->commands->args[j])
	{
		if (!is_valid_identifier(data->commands->args[j]))
		{
			print_export_error(data->commands->args[j]);
			g_exit_status = 1;
			j++;
			continue ;
		}
		if (strchr(data->commands->args[j], '=') != NULL)
		{
			update_env_var(data, data->commands->args[j]);
			g_exit_status = 0;
		}
		else
		{
			add_env_var_no_value(data, data->commands->args[j]);
			g_exit_status = 0;
		}
		j++;
	}
	return (g_exit_status);
}
