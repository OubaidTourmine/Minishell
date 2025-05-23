#include "../minishell.h"

int	read_hd_content_from_file(t_token *heredoc, char *filepath)
{
	char	buffer[4096];
	char	*tmp;
	int		bytesread;
	int		fd;

	fd = open(filepath, O_RDONLY, 0600);
	if (fd == -1)
		return (perror("minishell: open"), 1);
	heredoc->hd_content = ft_strdup("");
	bytesread = read(fd, buffer, 4095);
	while (bytesread > 0)
	{
		buffer[bytesread] = '\0';
		tmp = heredoc->hd_content;
		heredoc->hd_content = ft_strjoin(heredoc->hd_content, buffer);
		ft_free(tmp);
		bytesread = read(fd, buffer, 4095);
	}
	close(fd);
	unlink(filepath);
	return (0);
}

void	get_heredoc_input(t_data *data, t_token *heredoc, char *filepath)
{
	char	*line;
	char	*content;
	int		fd;

	fd = open(filepath, O_WRONLY | O_CREAT, 0600);
	if (fd == -1)
	{	
		perror("minishell: open");
		free_memory();
		exit(1);
	}
	content = ft_strdup("");
	while (1)
	{
		tracker(line = readline("> "), TRACKER_ADD);
		if (!line || !ft_strcmp(heredoc->next->content, line))
		{	
			if(!line)
			print_heredoc_warning(heredoc->next->content);
			break ;
		}
		content = process_hd_line(data, heredoc, line, content);
	}
	ft_putstr_fd(content, fd);
	close(fd);
}

int	handle_heredoc(t_data *data, t_token *heredoc)
{
	pid_t	pid;
	int		status;
	char	*filepath;
	void	(*saved_sig_handler)(int);

	filepath = generate_heredoc_filepath();
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	saved_sig_handler = signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, handle_child_sigint);
		get_heredoc_input(data, heredoc, filepath);
		free_memory();
		exit(0);
	}
		waitpid(pid, &status, 0);
		signal(SIGINT, saved_sig_handler);
		if (check_hd_exit_status(status)
			|| read_hd_content_from_file(heredoc, filepath))
			return (write(1, "\n", 1), 1);

	return (0);
}
int	process_heredocs(t_data *data)
{
	t_token *token;

	token = data->tokens;
	while (token)
	{
		if (check_syntax_errors(token))
			return (1);
		if (token->type == HEREDOC)
		{
			if (handle_heredoc(data, token))
				return (1);
			token = token->next->next;
		}
		else
			token = token->next;
	}
	return (0);
}