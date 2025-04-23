#ifndef MINISHELL_H
# define MINISHELL_H

#define TRACKER_ADD 0
#define TRACKER_GET 1

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_type
{
	WORD,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE
}						t_type;

typedef struct s_alloc_list
{
	void				*ptr;
	struct s_alloc_list	*next;
}						t_alloc_list;

typedef struct s_redirect
{
	t_type				type;
	char				*target;
	char				*hd_content;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	t_redirect			*redirects;
	int					has_invalid_redirect;
	struct s_command	*next;
}						t_command;

typedef struct s_token
{
	char				*content;
	char				*hd_content;
	t_type				type;
	struct s_token		*next;
	int					no_expansion;
	int					is_amb_redirect;
	int					has_invalid_file;
	char				*amb_error_msg;
	int					has_syntax_error;
}						t_token;

typedef struct s_data
{
	t_alloc_list		*allocs;
	t_token				*tokens;
	t_token				*last_token;
	t_command			*commands;
	char				*pwd;
	char				*old_pwd;
	char				**env;
	char				**export;
	int					*line_pos;
	int					exit_status;
	int					is_delimiter;
	int					is_filename;
	int					found_syntax_error;
}						t_data;

void					setup_signals(void);
void 		handle_heredoc_sigint(int sig);

void					ft_putstr_fd(char *s, int fd);
char					*ft_strdup(const char *s1);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_itoa(int n);
char					*add_char_to_str(char *str, char c);
int						ft_strcmp(const char *s1, const char *s2);
void					*ft_memset(void *b, int c, size_t len);
size_t					ft_strlen(const char *s);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);
int						is_valid_var_char(int c);
int						has_non_space_char(char *str);
int						is_expandable(int c);
int						ft_isspace(char c);
void					skip_spaces(char *line, int *i);

int						check_leading_pipe(char *line, int *i);
int						check_syntax_errors(t_token *token);
int						check_consecutive_operators(t_token *token);

int						tokenize_input(char *line, t_data *data);
t_token	*create_new_token(t_type type, char *content);
void					add_token_to_list(t_data *data, t_type type,
							char *content);
void					set_flags(t_data *data, int delimiter, int filename);

char					*expand(char *line, int *i, t_data *data);

void					append_to_last_token(t_data *data, char *str,
							int quoted);
void					check_lead_space(t_data *data, char *var_value);
void					check_trail_space(t_data *data, char *var_value,
							char *line, int *i);
int 					is_amb_redirect(t_token *redirect);
void					handle_spaces_in_var(t_data *data, char *var_value,
							int *j);

void					handle_regular_word(t_data *data, char *line, int *i);
void					handle_single_quotes(t_data *data, char *line, int *i);
void					handle_double_quotes(t_data *data, char *line, int *i);
void					handle_var_expansion(t_data *data, char *line, int *i);

void					parse_commands(t_data *data);

t_alloc_list	**tracker(void *ptr, int option);
void		free_memory(void);
void		ft_free(void *ptr);
void		*ft_malloc(size_t size);

int						is_redirection(t_type type);
void					add_command_to_list(t_data *data,
							t_command *new_command);
int						count_args(t_token *current);

int						process_heredocs(t_data *data);
char	*generate_heredoc_filepath(void);
void					print_heredoc_warning(char *delimiter);
int						check_hd_exit_status(t_data *data, int status);
int						handle_heredoc(t_data *data, t_token *heredoc);
char					*process_hd_line(t_data *data, t_token *hd, char *line,
							char *content);
void	mark_amb(char *line, int start_i, t_token *amb_redirect);
void					report_redirect_errors(t_data *data);
void					skip_to_next_command(t_token **token);

void	print_command_list(t_command *commands);// i need to remove this later
void    print_tokens(t_token *tokens);// also this...

typedef struct s_inf
{
	char				**env;
	char				**export;
	char				*command;
	char				*arg;
	char				*flag;
	char				*pipe;
	char				less;
	char				great;
}						t_inf;


int						execution_pars(t_data *data, char **env);
void					handle_redirections(t_data *data);
int						get_env(char **env, t_data *data);
char					*cmd_found(char **path);
char					**get_envi(t_data *data, char *path);
int						execution(t_data *data, int flag);
char					**ft_split(char const *s, char c);
int						ft_strcmp_ft(char *s1, char *s2);
int						ft_strncmp_ft(char *s1, char *s2, size_t n);
int						get_export(t_data *data);
size_t					ft_strlen_ft(char *s);
char					*ft_strjoin_ft(char *s1, char *s2);
char					*ft_strdup_ft(char *src);
char					*ft_substr_ft(char *s, unsigned int start, size_t len);
void					free_array(char **ptr);
int						func_export(t_data *data);

#endif