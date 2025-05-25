/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:19:40 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:19:42 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_dir_path(char *filepath)
{
	char	*slash;

	slash = strrchr(filepath, '/');
	if (!slash)
		return (ft_strdup("."));
	else if (slash == filepath)
		return (ft_strdup("/"));
	else
		return (ft_substr(filepath, 0, slash - filepath));
}
