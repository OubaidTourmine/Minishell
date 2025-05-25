/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:17:13 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/15 15:21:33 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	execution_pars(t_data *data)
{
	int	result;

	if (!data->commands)
	{
		return (-1);
	}
	if (data->commands->next)
	{
		result = handle_pipe(data);
		g_exit_status = result;
		return (result);
	}
	if (data->commands->redirects)
		handle_redirections(data);
	result = handle_cmd(data, 0);
	g_exit_status = result;
	return (result);
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
		hd_file_manager(NULL);
		reset_data(&data);
	}
}
