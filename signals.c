#include "minishell.h"

void handle_child_sigint(int sig)
{	
	(void)sig;
	free_memory();
	exit(130);
}

void handle_child_sigquit(int sig)
{
	(void)sig;
	free_memory();
	exit(131);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
}