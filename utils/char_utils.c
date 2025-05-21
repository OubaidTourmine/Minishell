#include "../minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int	is_valid_var_char(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int	is_expandable(int c)
{
	if (ft_isalpha(c) || c == '_' || c == '?')
		return (1);
	return (0);
}
