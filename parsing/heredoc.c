#include "../minishell.h"

void read_heredoc_from_pipe(t_data *data, t_redirect *heredoc, int *fds)
{
	char buffer[4096];
	char *tmp;
	int bytesread;

	heredoc->hd_content = ft_strdup("", data);
	while((bytesread = read(fds[0], buffer, 4095)) > 0)
	{
		buffer[bytesread] = '\0';
		tmp = heredoc->hd_content;
		heredoc->hd_content = ft_strjoin(heredoc->hd_content, buffer, data);
		ft_free(tmp, data);
	}
	close(fds[0]);
}

char *process_heredoc_line(t_data *data, t_redirect *redirect, char *line, char *content)
{
    int i;

    line = add_char_to_str(line, '\n', data);
    if (redirect->no_expansion)
    {
    content = ft_strjoin_free(content, line, data);
    return content;
    }
    else
    {
        i = 0;
        while(line[i])
        {
            if (line[i] == '$' && is_valid_var_start(line[i + 1]))
            content = ft_strjoin_free(content,
            expand_variable(line, &i, data), data);
            else
            content = add_char_to_str(content, line[i++], data);
        }
    }
    ft_free(line, data);
    return content;
}

void	read_heredoc(t_data *data, t_redirect *heredoc, int *fds)
{
	char	*line;
	char	*content;

	content = ft_strdup("", data);
	while (1)
	{
		track_ptr(line = readline("> "), data);
		if (!line)
		{
			write(2,
				"minishell: warning: here-document delimited by end-of-file (wanted `",
				68);
			write(2, heredoc->target, ft_strlen(heredoc->target));
			write(2, "')\n", 3);
			break ;
		}
		if (!ft_strcmp(heredoc->target, line))
		break ;
		content = process_heredoc_line(data, heredoc, line, content);
	}
	write(fds[1], content, ft_strlen(content));
	close(fds[1]);
    exit(0);
}

int	handle_heredoc(t_data *data, t_redirect *heredoc)
{
	pid_t pid;
	int fds[2];
	int status;

	status = 1337;
	if (pipe(fds) == -1)
		return (perror("minishell: pipe"), 1);
	if ((pid = fork()) == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
    {
        close(fds[0]);
        signal(SIGINT, SIG_DFL);
		read_heredoc(data, heredoc, fds);
    }
	else
	{
		close(fds[1]);
		waitpid(pid, &status, 0);
		read_heredoc_from_pipe(data, heredoc, fds);
		printf("status: %i\n", WTERMSIG(status));
	}
	return (0);
}