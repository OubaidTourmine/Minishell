#include "../minishell.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(unsigned char *)(s + i) = 0;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size, t_data *data)
{
	void	*s;

	if (size && count > (size_t)-1 / size)
		return (NULL);
	s = ft_malloc((count * size), data);
	if (!s)
		return (NULL);
	ft_bzero(s, count * size);
	return (s);
}

char	*ft_substr(char const *s, unsigned int start, size_t len, t_data *data)
{
	size_t	i;
	char	*substr;

	if (!s)
		return (NULL);
	i = 0;
	if (start >= ft_strlen(s))
	{
		substr = ft_calloc(1, sizeof(char), data);
		return (substr);
	}
	s += start;
	if (ft_strlen((s)) < len)
		len = ft_strlen(s);
	substr = (char *)ft_malloc((len + 1) * sizeof(char), data);
	if (!substr)
		return (NULL);
	while (s[i] && i < len)
	{
		substr[i] = s[i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
