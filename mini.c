/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:13:36 by outourmi          #+#    #+#             */
/*   Updated: 2025/06/20 16:58:20 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_x_permession(mode_t mode)
{
	return (mode & S_IXUSR);
}

int	func_pwd(t_data *data)
{
	if (!data->env)
		printf("%s\n", data->pwd);
	else
	{
		data->pwd = getenv("PWD");
		if (!data->pwd)
		{
			perror("getenv failed\n");
			return (1);
		}
		printf("%s\n", data->pwd);
	}
	return (0);
}

int	func_cd(t_data *data)
{
	const char	*path;
	struct stat	file_stat;

	if (!data->commands->args)
	{
		path = getenv("HOME");
		if (!path)
		{
			perror("getenv failed\n");
			return (1);
		}
		if (chdir(path) == -1)
		{
			perror("chdir failed\n");
			return (1);
		}
		return (0);
	}
	else if (!ft_strcmp_ft(data->commands->args[1], "-"))
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			perror("getenv failed\n");
			return (1);
		}
		if (chdir(path) == -1)
		{
			perror("chdir failed\n");
			return (1);
		}
		return (0);
	}
	else
	{
		path = data->commands->args[1];
		if (!path)
			return (1);
		if (stat(path, &file_stat) == -1)
		{
			perror("stat failed\n");
			return (1);
		}
		if (S_ISDIR(file_stat.st_mode) && check_x_permession(file_stat.st_mode))
		{
			if (chdir(path) == -1)
			{
				perror("chdir failed\n");
				return (1);
			}
			return (0);
		}
	}
	return (1);
}

int	func_echo(t_data *data)
{
	printf("%s\n", data->commands->args[1]);
	return (0);
}

int	func_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		// if (!data->env[i  +1])
		// {
		//     break ;
		// }
		printf("%s\n", data->env[i++]);
	}
	return (0);
}

int	func_exit(t_data *data)
{
	(void)data;
	if (data->env)
		free_array(data->env);
	if (data->export)
		free_array(data->export);
	free(data);
	exit(0);
}

size_t	ft_strlen_exp(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		return (0);
	}
	while (s[i] && s[i] != '=')
	{
		i++;
	}
	return (i);
}

int	func_unset(t_data *data)
{
	int		i;
	int		var_len;
	int		len_args;
	char	*var_name;
	int		j;

	i = 0;
	len_args = ft_strlen_ft(data->commands->args[0]);
	while (data->env[i])
	{
		var_name = data->env[i];
		var_len = ft_strlen_exp(var_name);
		if (strncmp(var_name, data->commands->args[1], len_args) == 0
			&& var_len == len_args)
		{
			// Free both arrays' entries at this position
			free(data->env[i]);
			free(data->export[i]);
			// Shift all subsequent elements
			j = i;
			while (data->env[j + 1])
			{
				data->env[j] = data->env[j + 1];
				data->export[j] = data->export[j + 1];
				j++;
			}
			// Null-terminate both arrays at the new end
			data->env[j] = NULL;
			data->export[j] = NULL;
			return (0);
		}
		i++;
	}
	return (0);
}

int	func_export(t_data *data)
{
	int	i;

	i = 0;
	get_export(data);
	if (!data->commands->args[1])
	{
		while (data->export[i + 1])
			printf("%s\n", data->export[i++]);
	}
	else
	{
		while (data->env[i])
			i++;
		data->env[i] = ft_strdup_ft(data->commands->args[1]);
		// data->env[i] = ft_strjoin_ft("declare -x ", data->commands->args);
		data->env[i + 1] = NULL;
		i = 0;
		// while (data->export[i])
		// {
		//     // if (!data->export[i  + 2])
		//     // {
		//     //     break ;
		//     // }
		//     // printf("%s\n", data->export[i++]);
		// }
	}
	return (0);
}

int	builtin(t_data *data)
{
	if (ft_strcmp_ft(data->commands->args[0], "echo") == 0)
		func_echo(data);
	else if (ft_strcmp_ft(data->commands->args[0], "cd") == 0)
		func_cd(data);
	else if (ft_strcmp_ft(data->commands->args[0], "pwd") == 0)
		func_pwd(data);
	else if (ft_strcmp_ft(data->commands->args[0], "env") == 0)
		func_env(data);
	else if (ft_strcmp_ft(data->commands->args[0], "export") == 0)
		func_export(data);
	else if (ft_strcmp_ft(data->commands->args[0], "unset") == 0)
		func_unset(data);
	else if (ft_strcmp_ft(data->commands->args[0], "exit") == 0)
		func_exit(data);
	return (0);
}

int	get_env(char **env, t_data *data)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
		i++;
	data->env = malloc((i) * sizeof(char *) + 8);
	if (!data->env)
		return (1);
	i = 0;
	while (env[i] != NULL)
	{
		data->env[i] = ft_strdup_ft(env[i]);
		i++;
	}
	data->env[i] = NULL;
	return (0);
}

// int execute_pipeline_with_redirections(t_data *data)
// {
// 	// For each command in pipeline:
// 	// Child process:
// 	//   1. Set up pipe connections (stdin from previous, stdout to next)
// 	//   2. Check current->redirects
// 	//   3. Apply redirections (override pipe connections)
// 	//   4. Close unused pipe fds
// 	//   5. execve()
// }

int	builtin_checker(t_data *data)
{
	if (ft_strcmp_ft(data->commands->args[0], "echo") == 0
		|| ft_strcmp_ft(data->commands->args[0], "cd") == 0
		|| ft_strcmp_ft(data->commands->args[0], "pwd") == 0
		|| ft_strcmp_ft(data->commands->args[0], "env") == 0
		|| ft_strcmp_ft(data->commands->args[0], "export") == 0
		|| ft_strcmp_ft(data->commands->args[0], "unset") == 0
		|| ft_strcmp_ft(data->commands->args[0], "exit") == 0)
		return (0);
	else
		return (1);
}

void	do_dup(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, 1);
		close(fd_out);
	}
}

size_t	countlen(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (set[j] && s1[i])
	{
		if (set[j] == s1[i])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	last_s1;

	if (!s1 || !set)
		return (0);
	j = 0;
	i = countlen(s1, set);
	last_s1 = ft_strlen(s1) - 1;
	if (i > last_s1)
		return (ft_substr(s1, 0, 0));
	while (set[j] && s1[last_s1])
	{
		if (set[j] == s1[last_s1])
		{
			last_s1--;
			j = 0;
		}
		else
			j++;
	}
	return (ft_substr(s1, i, last_s1 - i + 1));
}

char	*get_path(char **env)
{
	int		i;
	int		j;
	char	*name;

	j = 0;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (!ft_strncmp_ft(name, "PATH", 4))
		{
			free(name);
			return (env[i] + j + 1);
		}
		free(name);
		i++;
	}
	return (NULL);
}

char	*ft_check_command(char *cmd, char *path)
{
	char	**splited_path;
	int		i;
	char	*join_path;
	char	*join_cmd;

	i = 0;
	if (path == NULL)
		return (NULL);
	splited_path = ft_split(path, ':');
	while (splited_path[i])
	{
		join_path = ft_strjoin(splited_path[i], "/");
		join_cmd = ft_strjoin(join_path, cmd);
		if (!access(join_cmd, F_OK | X_OK))
		{
			free(join_path);
			ft_free(splited_path);
			return (join_cmd);
		}
		free(join_cmd);
		free(join_path);
		i++;
	}
	ft_free(splited_path);
	return (NULL);
}

char	**join_cmd_with_path(char **cmd, char **env)
{
	char	*path;
	char	**res;
	char	*new_cmd;

	res = cmd;
	path = get_path(env);
	new_cmd = ft_check_command(cmd[0], path);
	if (new_cmd != NULL)
	{
		if (access(new_cmd, F_OK | X_OK) != 0)
		{
			free(new_cmd);
			res = NULL;
		}
		else
		{
			free(cmd[0]);
			cmd[0] = new_cmd;
		}
	}
	else
		res = NULL;
	return (res);
}

char	**get_command(char *str, char **env)
{
	char	**cmd;
	char	**res;

	str = ft_strtrim(str, " ");
	cmd = ft_split(str, ' ');
	res = cmd;
	if (cmd == NULL)
		exit(1);
	if (cmd[0] == NULL)
		res = NULL;
	else if (cmd[0][0] == '.' || cmd[0][0] == '/')
	{
		if (access(cmd[0], F_OK | X_OK) != 0)
			res = NULL;
	}
	else
		res = join_cmd_with_path(cmd, env);
	free(str);
	if (!res)
		ft_free(cmd);
	return (res);
}

void	exec_cmd(t_data *data)
{
	char	**s_cmd;

	s_cmd = get_command(data->commands->args[0], data->env);
	if (s_cmd)
		execve(s_cmd[0], data->commands->args, data->env);
	else
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(data->commands->args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
}

int	ft_get_exit_status(int status)
{
	int	exit_status;

	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}

int	handle_cmd(t_data *data, int flag)
{
	int	pid;
	int	status;

	if (!builtin_checker(data))
		return (builtin(data));
	pid = fork();
	if (pid == 0)
	{
		do_dup(data->fd_in, data->fd_out);
		exec_cmd(data);
	}
	else if (pid < 0)
		perror("fork failed");
	else
		waitpid(pid, &status, 0);
	if (flag)
		exit(ft_get_exit_status(status));
	return (ft_get_exit_status(status));
}

size_t	get_how_many_pipe(t_data *data)
{
	size_t		cmd_count;
	t_command	*current;

	cmd_count = 0;
	current = data->commands;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}
	return (cmd_count);
}

void	get_in_ou_fd(t_data *data, t_data_pipe *p_data)
{
	if (p_data->i == 0)
	{
		data->fd_out = p_data->p_fd[1];
		close(p_data->p_fd[0]);
	}
	else if (p_data->i < p_data->count)
	{
		data->fd_in = p_data->stored_fd_in;
		data->fd_out = p_data->p_fd[1];
		close(p_data->p_fd[0]);
	}
	else if (p_data->i == p_data->count)
	{
		data->fd_in = p_data->stored_fd_in;
		if (!p_data->flag)
			data->fd_out = 1;
	}
}

int	handle_error(void)
{
	perror("fork failed");
	return (EXIT_FAILURE);
}

void	pipe_data_init(t_data_pipe *p_data, t_data *data)
{
	p_data->count = get_how_many_pipe(data) - 1;
	p_data->pid = malloc(sizeof(int) * (p_data->count + 1));
	p_data->for_close = -1;
	p_data->i = 0;
	p_data->flag = 0;
}
int	routine_proccess(t_data *data, t_data_pipe *p_data)
{
	if (data->commands->redirects)
	{
		p_data->stored_fd_out = data->fd_out;
		handle_redirections(data);
		if (p_data->i == p_data->count && p_data->stored_fd_out != data->fd_out)
			p_data->flag = 1;
	}
	if (p_data->i < p_data->count)
		pipe(p_data->p_fd);
	p_data->pid[p_data->i] = fork();
	if (p_data->pid[p_data->i] == 0)
	{
		get_in_ou_fd(data, p_data);
		handle_cmd(data, 1);
	}
	else if (p_data->pid[p_data->i] < 0)
		return (handle_error());
	p_data->stored_fd_in = p_data->p_fd[0];
	if (p_data->for_close != -1 && p_data->for_close != p_data->stored_fd_in)
		close(p_data->for_close);
	p_data->for_close = p_data->stored_fd_in;
	close(p_data->p_fd[1]);
	data->commands = data->commands->next;
	p_data->i++;
	return (EXIT_SUCCESS);
}

int	handle_pipe(t_data *data)
{
	t_data_pipe	p_data;
	int			cap_ret;
	int			status;

	pipe_data_init(&p_data, data);
	while (p_data.i <= p_data.count)
	{
		cap_ret = routine_proccess(data, &p_data);
		if (cap_ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	close(p_data.p_fd[0]);
	p_data.i = -1;
	p_data.count++;
	while (++(p_data.i) < p_data.count)
		waitpid(p_data.pid[p_data.i], &status, 0);
	return (ft_get_exit_status(status));
}

int	execution_pars(t_data *data)
{
	if (data->commands->next)
		return (handle_pipe(data));
	if (data->commands->redirects)
		handle_redirections(data);
	handle_cmd(data, 0);
	return (0);
}
