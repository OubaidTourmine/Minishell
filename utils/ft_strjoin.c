#include "../minishell.h"

char	*ft_strjoin(char const *s1, char const *s2, t_data *data)
{
	size_t	len;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)ft_malloc((len + 1) * sizeof(char), data);
	ft_strlcpy(str, s1, (ft_strlen(s1) + 1));
	ft_strlcpy((str + ft_strlen(s1)), s2, (ft_strlen(s2) + 1));
	return (str);
}
