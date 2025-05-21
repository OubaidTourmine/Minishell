#include "../minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (ft_isspace(line[*i]))
		(*i)++;
}
