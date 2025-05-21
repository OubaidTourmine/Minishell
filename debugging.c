#include "minishell.h"

const char	*get_token_type_name(t_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == INPUT)
		return ("INPUT_REDIRECTION");
	else if (type == OUTPUT)
		return ("OUTPUT_REDIRECTION");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == PIPE)
		return ("PIPE");
	return ("UNKNOWN");
}

// Function to print the tokens
void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("Token Type: %s, Content: %s\n",
			get_token_type_name(current->type), current->content);
		current = current->next;
	}
}



// Print the type of redirection in human-readable format
void print_redirection_type(t_type type)
{
    if (type == INPUT)
        printf("< (input)");
    else if (type == OUTPUT)
        printf("> (output)");
    else if (type == APPEND)
        printf(">> (append)");
    else if (type == HEREDOC)
        printf("<< (heredoc)");
    else
        printf("unknown redirection");
}

// Print a single redirection
void print_redirection(t_redirect *redirect)
{
    printf("\t\tRedirection: ");
    print_redirection_type(redirect->type);
    printf(" → \"%s\"\n", redirect->target ? redirect->target : "NULL");
    if(redirect->type == HEREDOC)
    printf("Heredoc content:\n%s", redirect->hd_content);
}

// Print all redirections for a command
void print_redirections(t_redirect *redirects)
{
    t_redirect *current;
    
    if (!redirects)
    {
        printf("\t\tNo redirections\n");
        return;
    }
    
    current = redirects;
    while (current)
    {
        print_redirection(current);
        current = current->next;
    }
}

// Print a single command with its arguments and redirections
void print_command(t_command *cmd)
{
    int i;
    
    
    // Print arguments
    if((cmd->has_invalid_redirect))
    printf("This command has redirect error!\n");
    else
    {
    printf("\tArguments:\n");
    if (!cmd->args)
    {
        printf("\t\tNo arguments\n");
    }
    else
    {
        i = 0;
        while (cmd->args[i])
        {
            printf("\t\targ[%d]: \"%s\"\n", i, cmd->args[i]);
            i++;
        }
    }
    
    // Print redirections
    printf("\tRedirections:\n");
    print_redirections(cmd->redirects);
    }
   
}

// Print the entire command list
void print_command_list(t_command *commands)
{
    int cmd_num;
    t_command *current;
    
    if (!commands)
    {
        printf("Command list is empty\n");
        return;
    }
    
    cmd_num = 1;
    current = commands;
    printf("Command List:\n");
    printf("=============\n");
    
    while (current)
    {
        printf("Command %d:\n", cmd_num++);
        print_command(current);
        current = current->next;
    }
    printf("=============\n");
}