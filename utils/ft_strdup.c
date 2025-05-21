#include "../minishell.h"

char	*ft_strdup(const char *s1, t_data *data)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char *)ft_malloc((ft_strlen(s1) + 1) * sizeof(char), data);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
