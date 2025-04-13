#include "../minishell.h"

void set_flags(t_data *data, int delimiter, int filename)
{
	data->is_delimiter = delimiter;
	data->is_filename = filename;
}

void	add_token_to_list(t_data *data, t_type type, char *content)
{
	t_token	*tmp;
	t_token	*token;

	if (!content)
		content = ft_strdup("", data);
	token = new_token(data, type, content);
	if (!token)
		return ;
	if (!data->tokens)
	{
		data->tokens = token;
		return ;
	}
	tmp = data->tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
}

t_token	*new_token(t_data *data, t_type type, char *content)
{
	t_token	*new_token;
	t_token	*tmp;

	if (type == WORD)
	{
		tmp = data->tokens;
		if (tmp)
		{
			while (tmp->next)
				tmp = tmp->next;
			if (!tmp->content[0])
				return (ft_free(content, data), NULL);
		}
	}
	new_token = (t_token *)ft_malloc(sizeof(t_token), data);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->content = content;
	new_token->type = type;
	return (new_token);
}
