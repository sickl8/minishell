# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/11 18:45:58 by isaadi            #+#    #+#              #
#    Updated: 2021/01/15 16:06:29 by isaadi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft

SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
ft_isdigit.c ft_isprint.c ft_itoa.c ft_lstadd_back_bonus.c \
ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c \
ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c \
ft_lstsize_bonus.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
ft_putstr_fd.c ft_split.c ft_strchr.c ft_strcmp.c ft_strdup.c ft_strjoin.c \
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strncpy.c \
ft_strnstr.c ft_strrchr.c ft_strstr.c ft_strtolower.c ft_strtrim.c ft_substr.c \
ft_tolower.c ft_toupper.c

all: $(NAME)

$(NAME):
	@gcc -Wall -Werror -Wextra -c $(SRC)
	@ar rc libft.a *.o
	@ranlib libft.a

clean:
	@/bin/rm -f *.o

fclean: clean
	@/bin/rm -f libft.a

bonus:
	@gcc -Wall -Werror -Wextra -c *_bonus.c
	@ar rc libft.a *.o
	@ranlib libft.a

re: fclean all
