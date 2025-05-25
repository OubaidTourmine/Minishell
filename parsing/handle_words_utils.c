/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_words_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:24:19 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/12 18:24:49 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_spaces_in_var(t_data *data, char *var_value, int *j)
{
	add_token_to_list(data, WORD, NULL);
	skip_spaces(var_value, j);
}

int	is_amb_redirect(t_token *redirect)
{
	int		valid_words;
	t_token	*current;

	current = redirect->next;
	valid_words = 0;
	while (current)
	{
		if (current->type == WORD && current->content)
			valid_words++;
		current = current->next;
	}
	if (valid_words != 1)
		return (1);
	else
		return (0);
}

void	append_to_last_token(t_data *data, char *str, int quoted)
{
	char	*old_content;

	if (!data->last_token->content)
		data->last_token->content = ft_strdup("");
	old_content = data->last_token->content;
	data->last_token->content = ft_strjoin_free(old_content, str);
	if (quoted && data->is_delimiter)
		data->last_token->no_expansion = 1;
}

// void	check_lead_space(t_data *data, char *var_value)
// {
// 	if (ft_isspace(var_value[0]))
// 	{
// 		if (data->last_token->content)
// 			add_token_to_list(data, WORD, NULL);
// 	}
// }

// void	check_trail_space(t_data *data, char *var_value, char *line, int *i)
// {
// 	int	j;

// 	j = *i;
// 	if (has_non_space_char(var_value)
// 		&& ft_isspace(var_value[ft_strlen(var_value) - 1]))
// 	{
// 		while (line[j] && line[j] != '|' && line[j] != '<' && line[j] != '>')
// 		{
// 			if (line[j] && !ft_isspace(line[j]))
// 			{
// 				add_token_to_list(data, WORD, NULL);
// 				break ;
// 			}
// 			j++;
// 		}
// 	}
// }