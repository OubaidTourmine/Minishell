/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:46:54 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/14 14:15:22 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	countlen(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (set[j] && s1[i])
	{
		if (set[j] == s1[i])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	last_s1;

	if (!s1 || !set)
		return (0);
	j = 0;
	i = countlen(s1, set);
	last_s1 = ft_strlen(s1) - 1;
	if (i > last_s1)
		return (ft_substr(s1, 0, 0));
	while (set[j] && s1[last_s1])
	{
		if (set[j] == s1[last_s1])
		{
			last_s1--;
			j = 0;
		}
		else
			j++;
	}
	return (ft_substr(s1, i, last_s1 - i + 1));
}

size_t	ft_strlen_exp(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		return (0);
	}
	while (s[i] && (s[i] != '+' && s[i] != '='))
	{
		i++;
	}
	return (i);
}

int	ft_get_exit_status(int status)
{
	int	exit_status;

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(2, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		exit_status = WTERMSIG(status) + 128;
	}
	else if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}

int	handle_error(void)
{
	ft_putstr_fd("fork failed", 2);
	return (EXIT_FAILURE);
}
