/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:21:20 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/14 19:11:41 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_swap(char **export, int i, int j)
{
	char	*tmp;

	tmp = export[i];
	export[i] = export[j];
	export[j] = tmp;
}

char	*ft_strdup_ft(char *src)
{
	int		i;
	int		len;
	char	*dest;

	i = 0;
	len = 0;
	if (src == NULL)
		return (NULL);
	while (src[len])
	{
		len++;
	}
	dest = ft_malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_atoi(char *nptr)
{
	int	i;
	int	signe;
	int	result;

	i = 0;
	result = 0;
	signe = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
	{
		i++;
	}
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			signe *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - 48;
		i++;
	}
	return (result * signe);
}
