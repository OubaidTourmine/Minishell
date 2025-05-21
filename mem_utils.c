#include "minishell.h"

void	free_memory(t_data *data)
{
	t_alloc_list	*next;
	int				saved_exit_status;

	while (data->allocs)
	{
		next = data->allocs->next;
		free(data->allocs->ptr);
		free(data->allocs);
		data->allocs = next;
	}
	saved_exit_status = data->exit_status;
	ft_memset(data, 0, sizeof(t_data));
	data->exit_status = saved_exit_status;
}

void	ft_free(void *ptr, t_data *data)
{
	t_alloc_list	*current;
	t_alloc_list	*previous;

	current = data->allocs;
	previous = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (previous)
				previous->next = current->next;
			else
				data->allocs = current->next;
			free(current->ptr);
			current->ptr = NULL;
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	track_ptr(void *ptr, t_data *data)
{
	t_alloc_list	*new_node;

	new_node = (t_alloc_list *)malloc(sizeof(t_alloc_list));
	if (!new_node)
	{
		free_memory(data);
		free(ptr);
		ft_putstr_fd("minishell: cannot allocate memory\n", 2);
		exit(1);
	}
	new_node->ptr = ptr;
	new_node->next = data->allocs;
	data->allocs = new_node;
}

void	*ft_malloc(size_t size, t_data *data)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		free_memory(data);
		perror("minishell: malloc");
		exit(1);
	}
	track_ptr(ptr, data);
	return (ptr);
}
