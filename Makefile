SRC = main.c sesco_builtins_1.c sesco_builtins_2.c sesco_builtins_3.c \
sesco_builtins_4.c sesco_builtins_5.c sesco_create_pipes.c sesco_execution.c \
sesco_ft_export.c sesco_redirections.c sesco_tools.c sickl_bwrite.c \
sickl_error_0.c sickl_find_0.c sickl_free_0.c sickl_free_1.c sickl_free_2.c \
sickl_get_next_line.c sickl_init_0.c sickl_init_1.c sickl_init_2.c \
sickl_macros_0.c sickl_macros_1.c sickl_macros_2.c sickl_macros_3.c \
sickl_macros_4.c sickl_mask_0.c sickl_mask_1.c sickl_mask_2.c sickl_split_0.c \
sickl_split_1.c sickl_split_2.c sickl_split_3.c sickl_tools_0.c \
sickl_tools_1.c sickl_tools_2.c sickl_mrdt_0.c sesco_builtins_6.c

WWW = -Wall -Wextra -Werror

NAME = minishell

INCLUDEF = /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/

all: $(NAME)

$(NAME):
	cd libft && make
	gcc -I$(INCLUDEF) $(WWW) $(SRC) libft/libft.a -o $(NAME)

clean:
	cd libft && make clean

fclean: clean
	cd libft && make fclean
	rm -rf $(NAME)
	rm -rf $(NAME).dSYM

re: fclean $(NAME)

ref: fclean
	gcc -fsanitize=address -g -I$(INCLUDEF) $(WWW) $(SRC) -o $(NAME)
