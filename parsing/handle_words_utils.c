#include "../minishell.h"

void	handle_spaces_in_var(t_data *data, char *var_value, int *j)
{
	skip_spaces(var_value, j);
	if (var_value[*j])
		add_token_to_list(data, WORD, NULL);
}

// void	remove_empty_token(t_data *data)
// {
// 	t_token	*current;
// 	t_token	*previous;

// 	previous = NULL;
// 	current = data->tokens;
// 	if (data->last_token->content)
// 		return ;
// 	while (current->next)
// 	{
// 		previous = current;
// 		current = current->next;
// 	}
// 	if (!previous)
// 	{
// 		data->tokens = NULL;
// 		data->last_token = NULL;
// 	}
// 	else
// 	{
// 		previous->next = NULL;
// 		data->last_token = previous;
// 	}
// 	ft_free(current->content, data);
// 	ft_free(current, data);
// }

int is_amb_redirect(t_token *redirect)
{
	int valid_words;
	t_token *current;

	current = redirect->next;
	valid_words = 0;
	while(current)
	{
		if(current->type == WORD && current->content)
		valid_words++;
		current = current->next;
	}
	if(valid_words != 1)
	return 1;
	else
	return 0;
}

void	append_to_last_token(t_data *data, char *str, int quoted)
{
	char	*old_content;

	if (!data->last_token->content)
		data->last_token->content = ft_strdup("", data);
	old_content = data->last_token->content;
	data->last_token->content = ft_strjoin_free(old_content, str, data);
	if (quoted && data->is_delimiter)
		data->last_token->no_expansion = 1;
}

void	check_lead_space(t_data *data, char *var_value)
{
	if (ft_isspace(var_value[0]))
	{
		if (data->last_token->type == WORD && data->last_token->content)
			add_token_to_list(data, WORD, NULL);
	}
}

void	check_trail_space(t_data *data, char *var_value, char *line, int *i)
{
	int	j;

	j = *i;
	if (has_non_space_char(var_value)
		&& ft_isspace(var_value[ft_strlen(var_value) - 1]))
	{
		while (line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>')
		{
			if (line[j] && !ft_isspace(line[j]))
			{
				add_token_to_list(data, WORD, NULL);
				break ;
			}
			j++;
		}
	}
}
