/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:58:22 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 10:46:03 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort(char **export)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (export[len])
	{
		len++;
	}
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp_ft(export[j], export[j + 1]) > 0)
				ft_swap(export, j, j + 1);
			j++;
		}
		i++;
	}
}

int	xdeclare(t_data *data)
{
	int		i;
	char	*temp;

	i = 0;
	while (data->export[i])
	{
		temp = data->export[i];
		data->export[i] = ft_strjoin_ft("declare -x ", data->export[i]);
		ft_free(temp);
		i++;
	}
	return (0);
}

int	quote(t_data *data, int i, int j)
{
	char	*temp;
	char	*value;
	char	*new_var;
	char	*quoted_value;

	while (data->export[i])
	{
		j = 0;
		while (data->export[i][j] && data->export[i][j] != '=')
			j++;
		if (data->export[i][j] == '=')
		{
			temp = ft_substr_ft(data->export[i], 0, j + 1);
			value = ft_strjoin_ft("\"", &data->export[i][j + 1]);
			quoted_value = ft_strjoin_ft(value, "\"");
			new_var = ft_strjoin_ft(temp, quoted_value);
			data->export[i] = new_var;
		}
		i++;
	}
	return (0);
}

int	get_export(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
		i++;
	data->export = ft_malloc(sizeof(char *) * (i + 100));
	i = 0;
	while (data->env[i])
	{
		data->export[i] = ft_strdup_ft(data->env[i]);
		i++;
	}
	data->export[i] = NULL;
	return (0);
}

char	**create_sorted_export_copy(t_data *data)
{
	int		i;
	char	**temp_export;

	i = 0;
	while (data->export[i])
		i++;
	temp_export = ft_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (data->export[i])
	{
		temp_export[i] = ft_strdup_ft(data->export[i]);
		i++;
	}
	temp_export[i] = NULL;
	sort(temp_export);
	return (temp_export);
}
