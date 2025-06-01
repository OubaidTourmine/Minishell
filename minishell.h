/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: outourmi <outourmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:32:52 by outourmi          #+#    #+#             */
/*   Updated: 2025/07/19 17:02:56 by outourmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TRACKER_ADD 0
# define TRACKER_GET 1

# include <fcntl.h>
# include <limits.h>
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

extern int					g_exit_status;

typedef enum e_type
{
	WORD,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE
}							t_type;

typedef struct s_alloc_list
{
	void					*ptr;
	struct s_alloc_list		*next;
}							t_alloc_list;

typedef struct s_heredoc_files
{
	char					*filepath;
	struct s_heredoc_files	*next;
}							t_heredoc_list;

typedef struct s_redirect
{
	t_type					type;
	char					*target;
	char					*hd_content;
	struct s_redirect		*next;
}							t_redirect;

typedef struct s_command
{
	char					*cmd;
	char					**args;
	t_redirect				*redirects;
	int						has_invalid_redirect;
	struct s_command		*next;
}							t_command;

typedef struct s_token
{
	char					*content;
	char					*hd_content;
	t_type					type;
	struct s_token			*next;
	int						no_expansion;
	int						is_amb_redirect;
	int						has_invalid_file;
	char					*amb_error_msg;
	int						has_syntax_error;
}							t_token;

typedef struct s_data_pipe
{
	size_t					count;
	size_t					i;
	int						for_close;
	int						stored_fd_in;
	int						st_fd_out; // stored_fd_out
	int						flag;
	int						*pid;
	int						p_fd[2];
}							t_data_pipe;

typedef struct s_data
{
	t_data_pipe				*p_data;
	t_alloc_list			*allocs;
	t_token					*tokens;
	t_token					*last_token;
	t_command				*commands;
	int						fd_in;
	int						fd_out;
	char					*pwd;
	char					*old_pwd;
	char					**env;
	char					**export;
	int						exit_status;
	int						is_delimiter;
	int						is_filename;
	int						found_syntax_error;
}							t_data;

void						setup_signals(void);
void						handle_sigint(int sig);
int							has_spaces(char *str);
char						**resize_env_array(char **env_array);
void						handle_child_sigint(int sig);
void						handle_child_sigquit(int sig);
int							ft_atoi(char *nptr);
void						ft_putstr_fd(char *s, int fd);
char						*ft_strdup(const char *s1);
char						*ft_strjoin_free(char *s1, char *s2);
char						*ft_strjoin(char const *s1, char const *s2);
char						*ft_substr(char const *s, unsigned int start,
								size_t len);
char						*ft_itoa(int n);
char						*get_dir_path(char *filepath);
char						*add_char_to_str(char *str, char c);
int							ft_strcmp(const char *s1, const char *s2);
void						*ft_memset(void *b, int c, size_t len);
size_t						ft_strlen(const char *s);
size_t						ft_strlcpy(char *dst, const char *src,
								size_t dstsize);
int							is_path(char *str);
int							is_valid_var_char(int c);
int							has_non_space_char(char *str);
char 						*extract_var(char *var);
char 						*append_var(char *existing_var, char *new_var);
int 						has_append_operator(char *var);
int							is_expandable(int c);
int							ft_isdigit(int c);
int							ft_isspace(char c);
void						skip_spaces(char *line, int *i);

int							check_leading_pipe(char *line, int *i);
int							check_syntax_errors(t_token *token);
int							check_consecutive_operators(t_token *token);
int							ft_get_index_env(char **env, char *target);
size_t						ft_strlen_exp(char *s);
int							tokenize_input(char *line, t_data *data);
t_token						*create_new_token(t_type type, char *content);
void						add_token_to_list(t_data *data, t_type type,
								char *content);
void						set_flags(t_data *data, int delimiter,
								int filename);

char						*expand(char *line, int *i, t_data *data);

void						append_to_last_token(t_data *data, char *str,
								int quoted);
void						check_lead_space(t_data *data, char *var_value);
void						check_trail_space(t_data *data, char *var_value,
								char *line, int *i);
int							is_amb_redirect(t_token *redirect);
void						handle_spaces_in_var(t_data *data, char *var_value,
								int *j);

void						handle_regular_word(t_data *data, char *line,
								int *i);
void						handle_single_quotes(t_data *data, char *line,
								int *i);
void						handle_double_quotes(t_data *data, char *line,
								int *i);
void						handle_var_expansion(t_data *data, char *line,
								int *i);

void						parse_commands(t_data *data);

t_alloc_list				**tracker(void *ptr, int option);
void						free_memory(void);
void						ft_free(void *ptr);
void						*ft_malloc(size_t size);
void						hd_file_manager(char *filepath);

int							is_redirection(t_type type);
void						add_command_to_list(t_data *data,
								t_command *new_command);
int							count_args(t_token *current);
int							process_heredocs(t_data *data);
char						*generate_heredoc_filepath(void);
void						print_heredoc_warning(char *delimiter);
int							check_hd_exit_status(int status);
int							handle_heredoc(t_data *data, t_token *heredoc);
char						*process_hd_line(t_data *data, t_token *hd,
								char *line, char *content);
void						mark_amb(char *line, int start_i,
								t_token *amb_redirect);
void						report_redirect_errors(t_data *data);
void						skip_to_next_command(t_token **token);
void						print_command_list(t_command *commands);
void						print_tokens(t_token *tokens);
typedef struct s_inf
{
	char					**env;
	char					**export;
	char					*command;
	char					*arg;
	char					*flag;
	char					*pipe;
	char					less;
	char					great;
}							t_inf;

int							func_pwd(void);
void						print_export_error(char *arg);
int							ft_get_exit_status(int status);
void						do_dup(int fd_in, int fd_out);
int							execution_pars(t_data *data);
void						handle_redirections(t_data *data);
int							get_env(char **env, t_data *data);
char						*cmd_found(char **path);
char						**get_envi(t_data *data, char *path);
int							execution(t_data *data, int flag);
char						**ft_split(char const *s, char c);
int							ft_strcmp_ft(char *s1, char *s2);
int							ft_strncmp_ft(char *s1, char *s2, size_t n);
int							get_export(t_data *data);
char						**get_formatted_export(t_data *data);
size_t						ft_strlen_ft(char *s);
char						*ft_strjoin_ft(char *s1, char *s2);
char						*ft_strdup_ft(char *src);
char						*ft_substr_ft(char *s, unsigned int start,
								size_t len);
int							func_export(t_data *data);
int							func_pwd(void);
int							func_env(t_data *data);
int							check_x_permession(mode_t mode);
int							func_echo(t_data *data);
int							func_cd(t_data *data);
int							func_unset(t_data *data);
int							func_exit(t_data *data);
char						*ft_strtrim(char const *s1, char const *set);
int							builtin_checker(t_data *data);
int							builtin(t_data *data);
int							handle_error(void);
int							is_valid_identifier(char *str);
void						print_export_error(char *arg);
void						setup_old_pwd(t_data *data);
int							cd_to_home(void);
int							cd_to_oldpwd(void);
int							cd_to_path(const char *path);
void						update_pwd_env(t_data *data);
void						ft_swap(char **export, int i, int j);
size_t						get_how_many_pipe(t_data *data);
void						get_in_ou_fd(t_data *data, t_data_pipe *p_data);
void						pipe_data_init(t_data_pipe *p_data, t_data *data);
void						update_env_var(t_data *data, char *arg);
int							routine_proccess(t_data *data, t_data_pipe *p_data);
char						**get_command(char *str, char **env);
char						**join_cmd_with_path(char **cmd, char **env);
char						*ft_check_command(char *cmd, char *path);
char						*get_path(char **env);
void						do_dup(int fd_in, int fd_out);
int							handle_pipe(t_data *data);
int							handle_cmd(t_data *data, int flag);
void						add_declare_prefix(char **temp_export);
void						add_quotes_to_values(char **temp_export);
char						**get_formatted_export(t_data *data);
char						**create_sorted_export_copy(t_data *data);
void						print_export_vars(t_data *data);
int							process_export_args(t_data *data);
#endif