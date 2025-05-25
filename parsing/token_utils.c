/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:19:48 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/12 18:21:31 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_flags(t_data *data, int delimiter, int filename)
{
	data->is_delimiter = delimiter;
	data->is_filename = filename;
}

void	add_token_to_list(t_data *data, t_type type, char *content)
{
	t_token	*new_token;

	new_token = create_new_token(type, content);
	if (!data->tokens)
	{
		data->tokens = new_token;
		data->last_token = new_token;
		return ;
	}
	data->last_token->next = new_token;
	if (!data->found_syntax_error)
	{
		data->last_token->has_syntax_error
			= check_consecutive_operators(data->last_token);
		if (data->last_token->has_syntax_error)
			data->found_syntax_error = 1;
	}
	data->last_token = new_token;
}

t_token	*create_new_token(t_type type, char *content)
{
	t_token	*new_token;

	new_token = (t_token *)ft_malloc(sizeof(t_token));
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->content = content;
	new_token->type = type;
	return (new_token);
}
