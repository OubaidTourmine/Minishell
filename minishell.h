#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef enum e_type
{
    WORD,
    INPUT,
    OUTPUT,
    HEREDOC,
    APPEND,
    PIPE
} t_type;


typedef struct s_alloc_list
{
    void *ptr;
    struct s_alloc_list *next;
}t_alloc_list;

typedef struct s_redirect
{
    t_type type;
    char *target;
    int no_expansion;
    char *hd_content;
    struct s_redirect *next;
} t_redirect;

typedef struct s_command
{
    char *cmd;
    char **args;
    t_redirect *redirects;
    struct s_command *next;
} t_command;

typedef struct s_token
{
    char *content;
    t_type type;
    struct s_token *next;
    int no_expansion;
    int is_amb_redirect;
}t_token;

typedef struct s_data
{
    t_alloc_list *allocs;
    t_token *tokens;
    t_token *last_token;
    t_command *commands;
    char *amb_msg;
    int exit_status;
    int is_delimiter;
    int is_filename;
    int stop_tokenization;
}t_data;

void setup_signals();

void	ft_putstr_fd(char *s, int fd);
char	*ft_strdup(const char *s1, t_data *data);
char	*ft_strdup(const char *s1, t_data *data);
char	*ft_strjoin_free(char *s1, char *s2, t_data *data);
char	*ft_strjoin(char const *s1, char const *s2, t_data *data);
char	*ft_substr(char const *s, unsigned int start, size_t len, t_data *data);
char    *add_char_to_str(char *str, char c, t_data *data);
int	    ft_strcmp(const char *s1, const char *s2);
void	*ft_memset(void *b, int c, size_t len);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int     is_valid_var_char(int c);
int has_non_space_char(char *str);
int is_valid_var_start(int c);
int ft_isspace(char c);
void  skip_spaces(char *line, int *i);


int check_unclosed_quotes(char *line, t_data *data);
int check_syntax_errors(t_data *data);
int	check_redirect_errors(t_data *data);

void tokenize_input(char *line, t_data *data);
t_token	*create_new_token(t_data *data, t_type type, char *content);
void	add_token_to_list(t_data *data, t_type type, char *content);
void set_flags(t_data *data, int delimiter, int filename);

char	*expand_variable(char *line, int *i, t_data *data);

void	append_to_last_token(t_data *data, char *str, int quoted);
void	check_lead_space(t_data *data, char *var_value);
void	check_trail_space(t_data *data, char *var_value, char *line, int *i);
void	remove_empty_token(t_data *data);
void    handle_spaces_in_var(t_data *data, char *var_value, int *j);

void	handle_regular_word(t_data *data, char *line, int *i);
void	handle_single_quotes(t_data *data, char *line, int *i);
void    handle_double_quotes(t_data *data, char *line, int *i);
void	handle_var_expansion(t_data *data, char *line, int *i);

int parse_commands(t_data *data);

void track_ptr(void *ptr, t_data *data);
void free_memory(t_data *data);
void ft_free(void *ptr, t_data *data);
void *ft_malloc(size_t size, t_data *data);

int is_redirection(t_type type);
void add_command_to_list(t_data *data, t_command *new_command);
int count_args(t_token *current);

int handle_heredoc(t_data *data, t_redirect *heredoc);
char	*generate_heredoc_filepath(t_data *data);
void print_heredoc_warning(char *delimiter);
int	check_heredoc_exit_status(t_data *data, int status);
char	*process_heredoc_line(t_data *data, t_redirect *redirect, char *line, char *content);

void mark_amb_redirect(t_data *data, char *line, t_token *amb_redirect);

void print_command_list(t_command *commands); // i need to remove this later
void	print_tokens(t_token *tokens); // also this...

#endif