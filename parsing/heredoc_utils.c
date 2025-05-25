/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:23:27 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/12 18:23:33 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*generate_heredoc_filepath(void)
{
	unsigned char	buffer[10];
	int				fd;
	char			*filepath;
	char			*char_set;
	int				i;

	char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (perror("minishell: open"), NULL);
	if (read(fd, buffer, 10) == -1)
		return (close(fd), perror("minishell: read"), NULL);
	filepath = (char *)ft_malloc((sizeof(char) * 24));
	i = 0;
	ft_strlcpy(filepath, "/tmp/heredoc_", 24);
	while (i < 10)
	{
		filepath[i + 13] = char_set[buffer[i] % 62];
		i++;
	}
	filepath[i + 13] = '\0';
	close(fd);
	return (filepath);
}

void	print_heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

int	check_hd_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		printf("dkhl hna");
		g_exit_status = WTERMSIG(status) + 128;
		return (1);
	}
	else if (WIFEXITED(status))
	{
		g_exit_status = WEXITSTATUS(status);
		if (g_exit_status == 0)
			return (0);
		else
			return (1);
	}
	g_exit_status = 1;
	return (1);
}

char	*process_hd_line(t_data *data, t_token *hd, char *line, char *content)
{
	int	i;

	line = add_char_to_str(line, '\n');
	if (hd->next->no_expansion)
	{
		content = ft_strjoin_free(content, line);
		return (content);
	}
	else
	{
		i = 0;
		while (line[i])
		{
			if (line[i] == '$' && is_expandable(line[i + 1]))
				content = ft_strjoin_free(content, expand(line, &i, data));
			else
				content = add_char_to_str(content, line[i++]);
		}
	}
	ft_free(line);
	return (content);
}
