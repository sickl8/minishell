SRC = sickl_tools_0.c sickl_tools_1.c sickl_tools_2.c ft_export.c \
sickl_macros_0.c sickl_macros_1.c sickl_macros_2.c sickl_macros_3.c \
sickl_macros_4.c libft/ft_tolower.c libft/ft_strtolower.c \
get_next_line.c main.c sesco_create_pipes.c sesco_execution.c \
sesco_redirections.c sesco_builtins_1.c sesco_builtins_5.c sesco_builtins_2.c \
sesco_builtins_3.c sesco_builtins_4.c sesco_tools.c libft/ft_atoi.c \
libft/ft_itoa.c libft/ft_strncpy.c libft/ft_strlen.c libft/ft_memset.c \
libft/ft_strchr.c libft/ft_strcmp.c libft/ft_strdup.c libft/ft_strstr.c \
bwrite.c libft/ft_strncmp.c libft/ft_memcmp.c libft/ft_strrchr.c \
libft/ft_strnstr.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strlcpy.c \
libft/ft_putnbr_fd.c libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c \
libft/ft_isdigit.c

SRCWM = sickl_tools_0.c sickl_tools_1.c sickl_tools_2.c ft_export.c \
sickl_macros_0.c sickl_macros_1.c sickl_macros_2.c sickl_macros_3.c \
sickl_macros_4.c libft/ft_tolower.c libft/ft_strtolower.c \
get_next_line.c sesco_create_pipes.c sesco_execution.c \
sesco_redirections.c sesco_builtins_1.c sesco_builtins_5.c sesco_builtins_2.c \
sesco_builtins_3.c sesco_builtins_4.c sesco_tools.c libft/ft_atoi.c \
libft/ft_itoa.c libft/ft_strncpy.c libft/ft_strlen.c libft/ft_memset.c \
libft/ft_strchr.c libft/ft_strcmp.c libft/ft_strdup.c libft/ft_strstr.c \
bwrite.c libft/ft_strncmp.c libft/ft_memcmp.c libft/ft_strrchr.c \
libft/ft_strnstr.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strlcpy.c \
libft/ft_putnbr_fd.c libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c \
libft/ft_isdigit.c

WWW = -Wall -Wextra -Werror

all: minishell

minishell:
	gcc $(WWW) $(SRC) -o minishell

clean:
	rm -rf *.o

fclean: clean
	rm -rf minishell*

re: fclean minishell

ref: fclean
	gcc -fsanitize=address -g $(WWW) $(SRC) -o minishell && export ASAN_OPTIONS=detect_leaks=1

run: ref
	./minishell

cc: fclean
	cclang -fsanitize=address -g $(WWW) $(SRC) -o minishell

crun: cc
	export ASAN_OPTIONS=detect_leaks=1 && ./minishell

leaks: fclean
	./compile.sh 3 LC main.c gcc $(WWW) $(SRCWM) -o minishell && ./minishell