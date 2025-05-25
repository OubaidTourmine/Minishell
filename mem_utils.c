/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:10:56 by ynadime           #+#    #+#             */
/*   Updated: 2025/07/15 15:22:10 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_file_manager(char *filepath)
{
	static t_heredoc_list	*file_list;
	t_heredoc_list			*new_file;

	if (filepath == NULL)
	{
		while (file_list)
		{
			unlink(file_list->filepath);
			file_list = file_list->next;
		}
		return ;
	}
	new_file = (t_heredoc_list *)ft_malloc(sizeof(t_heredoc_list));
	new_file->filepath = filepath;
	new_file->next = file_list;
	file_list = new_file;
}

t_alloc_list	**tracker(void *ptr, int option)
{
	static t_alloc_list	*alloc_lst;
	t_alloc_list		*new_node;

	if (option == TRACKER_GET)
		return (&alloc_lst);
	else if (option == TRACKER_ADD)
	{
		new_node = (t_alloc_list *)malloc(sizeof(t_alloc_list));
		if (!new_node)
		{
			free_memory();
			free(ptr);
			ft_putstr_fd("minishell: cannot allocate memory\n", 2);
			exit(1);
		}
		new_node->ptr = ptr;
		new_node->next = alloc_lst;
		alloc_lst = new_node;
	}
	return (NULL);
}

void	free_memory(void)
{
	t_alloc_list	*next;
	t_alloc_list	**alloc_lst;

	alloc_lst = tracker(NULL, TRACKER_GET);
	while (*alloc_lst)
	{
		next = (*alloc_lst)->next;
		free((*alloc_lst)->ptr);
		free((*alloc_lst));
		(*alloc_lst) = next;
	}
}

void	ft_free(void *ptr)
{
	t_alloc_list	*current;
	t_alloc_list	*previous;
	t_alloc_list	**alloc_lst;

	alloc_lst = tracker(NULL, TRACKER_GET);
	current = *alloc_lst;
	previous = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (previous)
				previous->next = current->next;
			else
				*alloc_lst = current->next;
			free(current->ptr);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		free_memory();
		perror("minishell: malloc");
		exit(1);
	}
	tracker(ptr, TRACKER_ADD);
	return (ptr);
}
