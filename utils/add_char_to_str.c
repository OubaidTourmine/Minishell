#include "../minishell.h"

char	*add_char_to_str(char *str, char c)
{
	char	*new_str;
	int		i;

	i = 0;
	if (!str)
		str = ft_strdup("");
	new_str = (char *)ft_malloc((ft_strlen(str) + 2) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	ft_free(str);
	return (new_str);
}
