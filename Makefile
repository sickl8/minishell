all: minishell

minishell:
	./compile.sh 3 LC main.c gcc libft/ft_strncpy.c libft/ft_strlen.c libft/ft_memset.c libft/ft_strchr.c libft/ft_strcmp.c libft/ft_strdup.c libft/ft_strstr.c bwrite.c libft/ft_strncmp.c libft/ft_memcmp.c libft/ft_strrchr.c libft/ft_strnstr.c -g -fsanitize=address -o minishell

clean:
	rm -rf *.o

fclean:
	rm -rf minishell

re: fclean minishell
