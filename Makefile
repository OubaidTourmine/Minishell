NAME = minishell

SRC = minishell.c \
      signals.c \
	  builtins.c \
	  builtins2.c \
	  builtins3.c \
	  builtins4.c \
	  builtins5.c \
	  builtins6.c \
	  builtins7.c \
      execution.c \
      export.c \
      export2.c \
      mini.c \
      split.c \
      redirection.c \
      shell_utils.c \
      shell_utils2.c \
      utils.c \
      utils2.c \
      utils3.c \
	  mem_utils.c \
	  parsing/syntax_errors.c \
	  parsing/redirect_errors.c \
	  parsing/expand.c \
	  parsing/handle_words.c \
	  parsing/handle_words_utils.c \
	  parsing/tokenizer.c \
	  parsing/token_utils.c \
	  parsing/parse_commands.c \
	  parsing/parse_commands_utils.c \
	  parsing/heredoc.c \
	  parsing/heredoc_utils.c \
	  utils/ft_putstr_fd.c \
	  utils/has_non_space_char.c \
	  utils/add_char_to_str.c \
	  utils/has_spaces.c \
	  utils/ft_strcmp.c \
	  utils/is_path.c \
	  utils/ft_itoa.c \
	  utils/ft_substr.c \
	  utils/ft_isspace.c \
	  utils/ft_strdup.c \
	  utils/ft_strjoin.c \
	  utils/ft_strjoin_free.c \
	  utils/ft_memset.c \
	  utils/ft_strlcpy.c \
	  utils/ft_strlen.c \
	  utils/char_utils.c \
	  utils/skip_spaces.c \
	  utils/get_dir_path.c \
	  
OBJS = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re