#include "minishell.h"

void	get_input(t_data *data)
{
	char	*line;
	track_ptr(line = readline("minishell: "), data);
	if (!line)
	{
	clear_history();
	exit(0);
	}
	else if(line[0])
	add_history(line);
	if (check_unclosed_quotes(line, data))
	return ;
	tokenize_input(line, data);
	if(check_syntax_errors(data) || check_redirect_errors(data) || parse_commands(data))
	return;
	free_memory(data);
}

int	main(void)
{
	t_data data;

	setup_signals();
	ft_memset(&data, 0, sizeof(t_data));
	while (1)
	{	
		
		get_input(&data);
	}
}