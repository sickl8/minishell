all: minishell

minishell:
	gcc -Wall -Werror -Wextra -o minishell

clean:
	rm -rf *.o

fclean:
	rm -rf minishell