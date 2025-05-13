#include "../minishell.h"

void add_redirection(t_data *data, t_token **current, t_command *command)
{
    t_redirect *redirects;
    t_redirect *new_redirect;

    new_redirect = (t_redirect *)ft_malloc(sizeof(t_redirect), data);
    ft_memset(new_redirect, 0, sizeof(t_redirect));
    new_redirect->type = (*current)->type;
    new_redirect->no_expansion = (*current)->next->no_expansion;
    new_redirect->target = ft_strdup((*current)->next->content, data);
    if(new_redirect->type == HEREDOC)
    handle_heredoc(data, new_redirect);
    (*current) = (*current)->next->next;
    if(!command->redirects)
    {
    command->redirects = new_redirect;
    return ;
    }
    redirects = command->redirects;
    while(redirects->next)
    redirects = redirects->next;
    redirects->next = new_redirect;
}

void set_args(t_data *data, t_command *command, int arg_count, t_token *current)
{
    char **args;
    int cmd_found;
    int i;
    
    i = 0;
    cmd_found = 0;
    args = (char **)ft_malloc(sizeof(char *) * (arg_count + 1), data);
    while(current && current->type != PIPE)
    {
        if(is_redirection(current->type))
        current = current->next->next;
        else if(current->type == WORD && cmd_found)
        {
        args[i++] = ft_strdup(current->content, data);
        current = current->next;
        }
        else if(current->type == WORD && !cmd_found)
        {
        cmd_found = 1;
        current = current->next;
        }
    }
    args[i] = NULL;
    command->args = args;
}

void initialize_command(t_data *data, t_command *command, t_token **cmd_start)
{
    t_token *current;
    int found_cmd;
    int arg_count;

    arg_count = 0;
    found_cmd = 0;
    current = *cmd_start;
    while(current && current->type != PIPE)
    {
        if(is_redirection(current->type))
        add_redirection(data, &current, command);
        else if(current->type == WORD && !found_cmd)
        {
        found_cmd = 1;
        command->cmd = ft_strdup(current->content, data);
        current = current->next;
        arg_count = count_args(current);
        }
        else
            current = current->next;
    }
    if(arg_count)
    set_args(data, command, arg_count, *cmd_start);
    (*cmd_start) = current;
}

void parse_commands(t_data *data)
{
    t_token *cmd_start;
    t_command *new_command;

    cmd_start = data->tokens;
    while(cmd_start)
    {   
        new_command = (t_command *)ft_malloc(sizeof(t_command), data);
        ft_memset(new_command, 0, sizeof(t_command));
        initialize_command(data, new_command, &cmd_start);
        add_command_to_list(data, new_command);
        if(cmd_start && cmd_start->type == PIPE)
        cmd_start = cmd_start->next;
    }
    print_command_list(data->commands);
}