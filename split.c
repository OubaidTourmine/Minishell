/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:40:01 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/19 15:08:34 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_freee(char **ptr)
{
	int	i;

	i = 0;
	if (ptr)
	{
		while (ptr[i])
		{
			free(ptr[i]);
			i++;
		}
		free(ptr);
	}
}

static int	ft_word_counter(char const *s, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (s[i])
	{
		while ((s[i] == c))
			i++;
		if (s[i])
			counter++;
		while ((s[i] != c) && s[i])
			i++;
	}
	return (counter);
}

static char	*ft_dup(char const *s, char c)
{
	int		len;
	int		i;
	char	*a;

	len = 0;
	i = 0;
	while ((s[len] != c) && s[len])
		len++;
	a = malloc(len + 1);
	if (a == NULL)
		return (NULL);
	while (i < len)
	{
		a[i] = s[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}

static char	**splitting(char const *s, char c, char **array)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (s[j])
	{
		while (s[j] == c)
			j++;
		if (s[j])
		{
			array[i] = ft_dup(s + j, c);
			if (array[i] == NULL)
			{
				ft_freee(array);
				return (NULL);
			}
			i++;
		}
		while ((s[j] != c) && s[j])
			j++;
	}
	array[i] = NULL;
	return (array);
}

char	**ft_split(char const *s, char c)
{
	char **array;
	int w_counter;

	if (s == NULL)
	{
		return (NULL);
	}
	w_counter = ft_word_counter(s, c);
	array = malloc(sizeof(char *) * (w_counter + 1));
	if (array == NULL)
	{
		return (NULL);
	}
	return (splitting(s, c, array));
}