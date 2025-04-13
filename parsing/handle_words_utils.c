#include "../minishell.h"

void handle_spaces_in_var(t_data *data, char *var_value, int *j)
{
	skip_spaces(var_value, j);
	if (var_value[*j])
	{
	if(data->is_filename)
	data->amb_redirect = 1;
	add_token_to_list(data, WORD, NULL);
	}
}

void	remove_empty_token(t_data *data)
{
	t_token	*current;
	t_token	*previous;

	current = data->tokens;
	if (!current->next && !current->content[0])
	{
		ft_free(current->content, data);
		ft_free(current, data);
		data->tokens = NULL;
	}
	else
	{
		while (current->next)
		{
			previous = current;
			current = current->next;
		}
		if (!current->content[0])
		{
			previous->next = NULL;
			ft_free(current->content, data);
			ft_free(current, data);
		}
	}
}

void	append_to_last_token(t_data *data, char *str, int quoted)
{
	t_token	*tmp;
	char	*old_content;

	tmp = data->tokens;
	while (tmp->next)
		tmp = tmp->next;
	old_content = tmp->content;
	tmp->content = ft_strjoin_free(old_content, str, data);
	if(quoted && data->is_delimiter)
	tmp->no_expansion = 1;
}

void	check_lead_space(t_data *data, char *var_value)
{
	t_token	*tmp;

	if (ft_isspace(var_value[0]))
	{
		tmp = data->tokens;
		while (tmp->next)
			tmp = tmp->next;
		if (tmp->type == WORD)
			add_token_to_list(data, WORD, NULL);
	}
}
void	check_trail_space(t_data *data, char *var_value, char *line, int *i)
{
	int	j;

	j = *i;
	if (var_value[0] && ft_isspace(var_value[ft_strlen(var_value) - 1]))
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
