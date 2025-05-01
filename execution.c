/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:46:49 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 21:24:14 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_envi(t_data *data, char *path)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (strncmp(path, data->env[i], ft_strlen_ft(path)) == 0)
		{
			return (ft_split(data->env[i] + 5, ':'));
		}
		i++;
	}
	return (0);
}
