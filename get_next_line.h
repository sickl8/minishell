/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 17:38:23 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/17 20:39:10 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct	s_chr
{
	char	*str;
	char	*t1;
	char	*t2;
	int		ret;
}				t_chr;

char			*ft_subs(const char *s, unsigned int start, size_t len);
char			*ft_sjoin(const char *s1, const char *s2);
int				f1(char **p1, char **p2, int x);
int				get_next_line(int fd, char **line);
char			*fs(const char *str, int x);
char			*ft_sdup(const char *s1);
size_t			ft_slen(const char *s);

#endif
