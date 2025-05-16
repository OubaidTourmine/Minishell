#include "../minishell.h"

int	check_unclosed_quotes(char *line, t_data *data)
{
	int		i;
	char	quotes;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quotes = line[i];
			i++;
			while (line[i] && line[i] != quotes)
				i++;
			if (!line[i])
			{
				ft_free(line, data);
				ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}
int check_pipe_syntax(t_token *token)
{
	
	if((token->type == PIPE) && (!token->next || token->next->type == PIPE))
	{
	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	return 1;
	}
	return 0;
}
void print_redirection_error(t_type type)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if(type == PIPE)
	ft_putstr_fd("`|'\n", 2);
	else if(type == INPUT)
	ft_putstr_fd("`<'\n", 2);
	else if(type == OUTPUT)
	ft_putstr_fd("`>'\n", 2);
	else if(type == HEREDOC)
	ft_putstr_fd("`<<'\n", 2);
	else if(type == APPEND)
	ft_putstr_fd("`>>'\n", 2);
}
int check_redirection_syntax(t_token *token)
{

	if(token->type == INPUT || token->type == OUTPUT ||
	token->type == HEREDOC || token->type == APPEND)
		{
			if(token->is_amb_redirect)
			return 0;
			if(!token->next)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return 1;
			}
			else if(token->next->type != WORD)
			{
			print_redirection_error(token->next->type);
			return 1;
			}
		}
	return 0;
}

int check_syntax_errors(t_data *data)
{
	t_token *tokens;

	tokens = data->tokens;
	if(tokens && tokens->type == PIPE)
	{
	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	free_memory(data);
	return 1;
	}
	while(tokens)
	{
		if(check_pipe_syntax(tokens) || check_redirection_syntax(tokens))
		{
		free_memory(data);
		return 1;
		}
		tokens = tokens->next; 
	}
	return 0;
}
