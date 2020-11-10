# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isaadi <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/11 18:45:58 by isaadi            #+#    #+#              #
#    Updated: 2019/10/20 18:58:45 by isaadi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = exec

all: $(NAME)

$(NAME):
	@gcc -Wall -Werror -Wextra -c ft_memset.c ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c
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
