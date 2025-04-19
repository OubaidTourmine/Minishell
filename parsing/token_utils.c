#include "../minishell.h"

void set_flags(t_data *data, int delimiter, int filename)
{
	data->is_delimiter = delimiter;
	data->is_filename = filename;
}

void	add_token_to_list(t_data *data, t_type type, char *content)
{
	t_token	*new_token;

	new_token = create_new_token(data, type, content);
	if (!data->tokens)
	{
		data->tokens = new_token;
		data->last_token = new_token;
		return ;
	}
	data->last_token->next = new_token;
	data->last_token = new_token;
}

t_token	*create_new_token(t_data *data, t_type type, char *content)
{
	t_token	*new_token;

	new_token = (t_token *)ft_malloc(sizeof(t_token), data);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->content = content;
	new_token->type = type;
	return (new_token);
}
