#include "../minishell.h"

int add_redirection(t_data *data, t_token **current, t_command *command)
{
    t_redirect *redirects;
    t_redirect *new_redirect;

    new_redirect = (t_redirect *)ft_malloc(sizeof(t_redirect), data);
    ft_memset(new_redirect, 0, sizeof(t_redirect));
    new_redirect->type = (*current)->type;
    new_redirect->no_expansion = (*current)->next->no_expansion;
    new_redirect->target = ft_strdup((*current)->next->content, data);
    if(new_redirect->type == HEREDOC)
    {
    if(handle_heredoc(data, new_redirect))
    return 1;
    }
    (*current) = (*current)->next->next;
    if(!command->redirects)
    {
    command->redirects = new_redirect;
    return 0;
    }
    redirects = command->redirects;
    while(redirects->next)
    redirects = redirects->next;
    redirects->next = new_redirect;
    return 0;
}

void set_args(t_data *data, t_command *command, int arg_count, t_token *current)
{
    char **args;
    int i;
    
    i = 0;
    args = (char **)ft_malloc(sizeof(char *) * (arg_count + 1), data);
    while(current && current->type != PIPE)
    {
        if(is_redirection(current->type))
        current = current->next->next;
        else if(current->type == WORD)
        {
        args[i++] = ft_strdup(current->content, data);
        current = current->next;
        }
    }
    args[i] = NULL;
    command->args = args;
}

int initialize_command(t_data *data, t_command *command, t_token **cmd_start)
{
    t_token *current;
    int arg_count;

    arg_count = 0;
    current = *cmd_start;
    while(current && current->type != PIPE)
    {
        if(is_redirection(current->type))
        {
        if(add_redirection(data, &current, command))
        return 1;
        }
        else if(current->type == WORD)
        {
        current = current->next;
        arg_count++;
        }
        else
            current = current->next;
    }
    if(arg_count)
    set_args(data, command, arg_count, *cmd_start);
    (*cmd_start) = current;
    return 0;
}

int parse_commands(t_data *data)
{
    t_token *cmd_start;
    t_command *new_command;

    cmd_start = data->tokens;
    while(cmd_start)
    {   
        new_command = (t_command *)ft_malloc(sizeof(t_command), data);
        ft_memset(new_command, 0, sizeof(t_command));
        if(initialize_command(data, new_command, &cmd_start))
        {
        free_memory(data);
        return 1;
        }
        add_command_to_list(data, new_command);
        if(cmd_start && cmd_start->type == PIPE)
        cmd_start = cmd_start->next;
    }
    // print_command_list(data->commands);
    return 0;
}