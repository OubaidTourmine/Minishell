#include "minishell.h"


void	reset_data(t_data *data)
{
    int		saved_exit_status;
    char	**saved_env;
    char	**saved_export;

    saved_exit_status = data->exit_status;
    saved_env = data->env;
    saved_export = data->export;
    
    ft_memset(data, 0, sizeof(t_data));
    
    data->exit_status = saved_exit_status;
    data->env = saved_env;
    data->export = saved_export;
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
	if (process_heredocs(data))
		return ;
	report_redirect_errors(data);
	parse_commands(data);
}

int	main(int ac, char **av, char **env)
{
    t_data	data;

    (void)ac;
    (void)av;
    setup_signals();
    ft_memset(&data, 0, sizeof(t_data));
    if (get_env(env, &data) != 0)
    {
        perror("Failed to initialize environment variables");
        return (EXIT_FAILURE);
    }
    
    if (get_export(&data) != 0)
    {
        perror("Failed to initialize export variables");
        return (EXIT_FAILURE);
    }

    while (1)
    {
        data.fd_in = 0;
        data.fd_out = 1;
        get_input(&data);
        execution_pars(&data);
        free_memory();
        reset_data(&data);
    }
}