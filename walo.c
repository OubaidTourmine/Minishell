/*
int	main(int ac, char **av, char **env)
{
    t_data	data;

    (void)ac;
    (void)av;
    setup_signals();
    ft_memset(&data, 0, sizeof(t_data));
    
    // Initialize environment and export variables once
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
        execution_pars(&data, env);
        free_memory();
        reset_data(&data);
    }
}

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

int	execution_pars(t_data *data, char **env)
{
    t_command	*current;
    int			has_pipeline;
    int			has_redirects;

    (void)env;
    
    // Check if pipeline has any redirects
    current = data->commands;
    has_pipeline = (data->commands->next != NULL);
    has_redirects = 0;
    while (current)
    {
        if (current->redirects)
        {
            has_redirects = 1;
            break ;
        }
        current = current->next;
    }
    if (has_pipeline && has_redirects)
    {
        execution(data, 1);
    }
    if (data->commands && data->commands->args && data->commands->args[0]
        && !(data->commands->next) && !(data->commands->redirects)
        && (ft_strcmp_ft(data->commands->args[0], "echo") == 0
            || ft_strcmp_ft(data->commands->args[0], "cd") == 0
            || ft_strcmp_ft(data->commands->args[0], "pwd") == 0
            || ft_strcmp_ft(data->commands->args[0], "env") == 0
            || ft_strcmp_ft(data->commands->args[0], "export") == 0
            || ft_strcmp_ft(data->commands->args[0], "unset") == 0
            || ft_strcmp_ft(data->commands->args[0], "exit") == 0))
    {
        builtin(data);
    }
    else if (data->commands && data->commands->args && data->commands->args[0]
        && !(data->commands->redirects))
        execution(data, 0);
    else if (data->commands->redirects)
        handle_redirections(data);
    return (0);
}
*/