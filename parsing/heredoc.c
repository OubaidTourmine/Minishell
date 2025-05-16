#include "../minishell.h"


int	read_hd_content_from_file(t_data *data, t_redirect *heredoc, char *filepath)
{
	char	buffer[4096];
	char	*tmp;
	int		bytesread;
	int		fd;

	fd = open(filepath, O_RDONLY, 0600);
	if (fd == -1)
		return (perror("minishell: open"), 1);
	heredoc->hd_content = ft_strdup("", data);
	while ((bytesread = read(fd, buffer, 4095)) > 0)
	{
		buffer[bytesread] = '\0';
		tmp = heredoc->hd_content;
		heredoc->hd_content = ft_strjoin(heredoc->hd_content, buffer, data);
		ft_free(tmp, data);
	}
	close(fd);
	unlink(filepath);
	return (0);
}


int	read_heredoc_input(t_data *data, t_redirect *heredoc, char *filepath)
{
	char	*line;
	char	*content;
	int		fd;

	fd = open(filepath, O_WRONLY | O_CREAT, 0600);
	if (fd == -1)
		return (perror("minishell: open"), 1);
	content = ft_strdup("", data);
	while (1)
	{
		track_ptr(line = readline("> "), data);
		if (!line)
		{
			print_heredoc_warning(heredoc->target);
			break ;
		}
		if (!ft_strcmp(heredoc->target, line))
			break ;
		content = process_heredoc_line(data, heredoc, line, content);
	}
	ft_putstr_fd(content, fd);
	close(fd);
	return (0);
}

int	handle_heredoc(t_data *data, t_redirect *heredoc)
{
	pid_t pid;
	int status;
	char *filepath;
	void (*saved_sig_handler)(int);
	

	filepath = generate_heredoc_filepath(data);
	if ((pid = fork()) == -1)
		return (perror("minishell: fork"), 1);
	saved_sig_handler = signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{	
		signal(SIGINT, SIG_DFL);
		if(read_heredoc_input(data, heredoc, filepath))
		exit(1);
		exit(0);
	}
	else
	{	
		waitpid(pid, &status, 0);
		signal(SIGINT, saved_sig_handler);
		if (check_heredoc_exit_status(data, status) || read_hd_content_from_file(data, heredoc, filepath))
		return(1);
	}
	return (0);
}