#include "../minishell.h"

char	*add_char_to_str(char *str, char c, t_data *data)
{
	char	*new_str;
	int		i;

	i = 0;
	if (!str)
		str = ft_strdup("", data);
	new_str = (char *)ft_malloc((ft_strlen(str) + 2) * sizeof(char), data);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	ft_free(str, data);
	return (new_str);
}
