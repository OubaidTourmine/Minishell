/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:12:44 by outourmi          #+#    #+#             */
/*   Updated: 2025/05/23 17:40:31 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen_ft(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		return (0);
	}
	while (s[i])
	{
		i++;
	}
	return (i);
}

char	*ft_substr_ft(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	length;
	char	*result;

	i = 0;
	if (s == NULL)
		return (NULL);
	length = ft_strlen_ft(s);
	if (start >= length)
		return (ft_strdup_ft(""));
	if (length - start < len)
	{
		len = length - start;
	}
	result = (char *)malloc(len + 1);
	if (result == NULL)
		return (NULL);
	while (i < len)
	{
		result[i] = s[start];
		i++;
		start++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strjoin_ft(char *s1, char *s2)
{
	int		i;
	int		h;
	int		len;
	char	*ss;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	h = 0;
	len = (ft_strlen_ft(s1) + ft_strlen_ft(s2));
	ss = malloc((len + 1) * sizeof(char));
	if (ss == NULL)
		return (NULL);
	while (s1[h])
	{
		ss[i++] = s1[h++];
	}
	h = 0;
	i = ft_strlen_ft(s1);
	while (s2[h])
	{
		ss[i++] = s2[h++];
	}
	ss[i] = '\0';
	return (ss);
}

int	ft_strcmp_ft(char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] || s2[j])
	{
		if (s1[i] != s2[j])
			return (s1[i] - s2[j]);
		i++;
		j++;
	}
	return (s1[i] - s2[i]);
}

int	ft_strncmp_ft(char *s1, char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*s22;
	unsigned char	*s11;

	s22 = ((unsigned char *)s2);
	s11 = ((unsigned char *)s1);
	i = 0;
	if (n == 0)
		return (0);
	while (s11[i] && s22[i] && (s11[i] == s22[i]) && (i < n))
	{
		i++;
	}
	if (s11[i] > s22[i] && (i < n))
	{
		return (1);
	}
	if (s11[i] < s22[i] && (i < n))
	{
		return (-1);
	}
	else
		return (0);
}

char	*ft_strdup_ft(char *src)
{
	int i;
	int len;
	char *dest;

	i = 0;
	len = 0;
	if (src == NULL)
		return (NULL);
	while (src[len])
	{
		len++;
	}
	dest = malloc((len + 1) * sizeof(char));
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