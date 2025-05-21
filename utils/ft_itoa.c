/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynadime <ynadime@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:34 by ynadime           #+#    #+#             */
/*   Updated: 2025/05/19 17:56:57 by ynadime          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	count_digits(int n)
{
	size_t	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	fillstr(char *str, size_t sign, size_t digits, int n)
{
	str[digits + sign] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (digits > 0)
	{
		str[digits + sign - 1] = (n % 10) + '0';
		n /= 10;
		digits--;
	}
}

char	*ft_itoa(int n, t_data *data)
{
	char	*str;
	size_t	sign;
	size_t	f;

	f = 0;
	sign = 0;
	if (n < 0)
	{
		sign++;
		if (n == -2147483648)
		{
			n++;
			f++;
		}
	}
	str = (char *)ft_malloc((count_digits(n) + sign + 1) * sizeof(char), data);
	if (!str)
		return (NULL);
	fillstr(str, sign, count_digits(n), n);
	if (f == 1)
		str[count_digits(n) + sign - 1]++;
	return (str);
}
