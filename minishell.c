#include "minishell.h"

void	get_input(void)
{
	char	*line;
	t_data data;

	ft_memset(&data, 0, sizeof(data));
	track_ptr(line = readline("minishell: "), &data);
	if (!line)
	{
	clear_history();
	exit(0);
	}
	else if(line[0])
	add_history(line);
	if (check_unclosed_quotes(line))
	return ;
	tokenize_input(line, &data);
	if(check_syntax_error(&data) || check_amb_redirect(&data))
	return;
	parse_commands(&data);
	free_memory(&data);
}

int	main(void)
{
	setup_signals();
	while (1)
	{
		get_input();
	}
}