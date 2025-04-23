#include "minishell.h"

void	reset_data(t_data *data)
{
	int	saved_exit_status;

	saved_exit_status = data->exit_status;
	ft_memset(data, 0, sizeof(t_data));
	data->exit_status = saved_exit_status;
}

void	get_input(t_data *data)
{
	char	*line;

	tracker(line = readline("minishell: "), TRACKER_ADD);
	if (!line)
	{
		free_memory();
		clear_history();
		exit(0);
	}
	else if (line[0])
		add_history(line);
	if (tokenize_input(line, data))
		return ;
	// print_tokens(data->tokens);
	if (process_heredocs(data))
		return ;
	report_redirect_errors(data);
	parse_commands(data);
	// print_command_list(data->commands);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	(void)ac;
	(void)av;
	setup_signals();
	ft_memset(&data, 0, sizeof(t_data));
	while (1)
	{
		get_input(&data);
		execution_pars(&data, env);
		free_memory();
		reset_data(&data);
	}
}