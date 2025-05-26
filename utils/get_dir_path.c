/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:19:40 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/18 16:02:57 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
		{
			return ((char *)s + i);
		}
		i--;
	}
	return (0);
}

char	*get_dir_path(char *filepath)
{
	char	*slash;

	slash = ft_strrchr(filepath, '/');
	if (!slash)
		return (ft_strdup("."));
	else if (slash == filepath)
		return (ft_strdup("/"));
	else
		return (ft_substr(filepath, 0, slash - filepath));
}
