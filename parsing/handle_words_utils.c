/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_words_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:22:54 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/19 16:22:55 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_spaces_in_var(t_data *data, char *var_value, int *j)
{
	if (!data->last_token || data->last_token->content)
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
