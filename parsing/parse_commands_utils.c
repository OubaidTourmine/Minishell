#include "../minishell.h"

int	is_redirection(t_type type)
{
	if (type == INPUT || type == OUTPUT || type == APPEND || type == HEREDOC)
		return (1);
	return (0);
}

void	add_command_to_list(t_data *data, t_command *new_command)
{
	t_command	*commands;

	if (!data->commands)
	{
		data->commands = new_command;
		return ;
	}
	commands = data->commands;
	while (commands->next)
		commands = commands->next;
	commands->next = new_command;
}

int	count_args(t_token *current)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type != PIPE)
	{
		if (is_redirection(current->type))
			current = current->next->next;
		else if (current->type == WORD)
		{
			arg_count++;
			current = current->next;
		}
	}
	return (arg_count);
}
