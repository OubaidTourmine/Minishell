/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:49:22 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 10:46:22 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_declare_prefix(char **temp_export)
{
	int		i;
	char	*temp;

	i = 0;
	while (temp_export[i])
	{
		temp = temp_export[i];
		temp_export[i] = ft_strjoin_ft("declare -x ", temp_export[i]);
		ft_free(temp);
		i++;
	}
}

void	add_quotes_to_values(char **temp_export)
{
	int		i;
	int		j;
	char	*temp;
	char	*value;
	char	*quoted_value;

	i = 0;
	while (temp_export[i])
	{
		j = 0;
		while (temp_export[i][j] && temp_export[i][j] != '=')
			j++;
		if (temp_export[i][j] == '=')
		{
			temp = ft_substr_ft(temp_export[i], 0, j + 1);
			value = ft_strjoin_ft("\"", &temp_export[i][j + 1]);
			quoted_value = ft_strjoin_ft(value, "\"");
			ft_free(temp_export[i]);
			temp_export[i] = ft_strjoin_ft(temp, quoted_value);
			ft_free(temp);
			ft_free(value);
			ft_free(quoted_value);
		}
		i++;
	}
}

char	**get_formatted_export(t_data *data)
{
	char	**temp_export;

	temp_export = create_sorted_export_copy(data);
	add_declare_prefix(temp_export);
	add_quotes_to_values(temp_export);
	return (temp_export);
}

int	func_export(t_data *data)
{
	if (!data->export)
		get_export(data);
	if (!data->commands->args[1])
		print_export_vars(data);
	else
		g_exit_status = process_export_args(data);
	return (g_exit_status);
}
