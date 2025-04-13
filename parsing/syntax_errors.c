#include "../minishell.h"

int	check_unclosed_quotes(char *line)
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
				free(line);
				write(2, "minishell: syntax error: unexpected end of file\n",
					48);
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
	write(2, "minishell: syntax error near unexpected token `|'\n", 51);
	return 1;
	}
	return 0;
}
void print_redirection_error(t_type type)
{
	write(2, "minishell: syntax error near unexpected token ", 46);
	if(type == PIPE)
	write(2, "`|'\n", 4);
	else if(type == INPUT)
	write(2, "`<'\n", 4);
	else if(type == OUTPUT)
	write(2, "`>'\n", 4);
	else if(type == HEREDOC)
	write(2, "`<<'\n", 5);
	else if(type == APPEND)
	write(2, "`>>'\n", 5);
}
int check_redirection_syntax(t_token *token)
{

	if(token->type == INPUT || token->type == OUTPUT ||
	token->type == HEREDOC || token->type == APPEND)
		{
			if(!token->next)
			{
				write(2, "minishell: syntax error near unexpected token `newline'\n", 56);
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

int check_syntax_error(t_data *data)
{
	t_token *tokens;

	tokens = data->tokens;
	if(tokens && tokens->type == PIPE)
	{
	write(2, "minishell: syntax error near unexpected token `|'\n", 51);
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
