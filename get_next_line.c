/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 21:20:58 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/17 20:38:11 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_ft2(t_chr *s, char **line)
{
	if (s->ret < BUFFER_SIZE)
	{
		s->t2 = s->str;
		s->str = fs(s->str, 0);
		if (!(*line = ft_subs(s->t2, 0, s->str - s->t2)))
			return (f1(&s->t2, NULL, -1));
		s->ret = -2;
		return (f1(&s->t2, NULL, 0));
	}
	s->t2 = s->str;
	s->str = fs(s->str, '\0');
	if (!(*line = ft_subs(s->t2, 0, s->str - s->t2 - 1)))
		return (f1(&s->t2, NULL, -1));
	return (f1(&s->t2, NULL, 1));
}

int		f(t_chr *s, char **line)
{
	if (s->ret == -2)
	{
		*line = ft_sdup("");
		return (f1(NULL, NULL, 0));
	}
	if (fs(s->str, '\n'))
	{
		s->t2 = s->str;
		s->str = fs(s->str, '\n') + 1;
		if (!(*line = ft_subs(s->t2, 0, s->str - s->t2 - 1)))
			return (f1(&s->t2, NULL, -1));
		if (!(s->str = ft_sdup(s->str)))
			return (f1(&s->t2, NULL, -1));
		return (ft_slen(s->t2) ? f1(&s->t2, NULL, 1)
			: f1(&s->t2, NULL, 0));
	}
	else
		return (ft_ft2(s, line));
}

int		f1(char **p1, char **p2, int x)
{
	if (p1)
	{
		free(*p1);
		*p1 = NULL;
	}
	if (p2)
	{
		free(*p2);
		*p2 = NULL;
	}
	return (x);
}

void	vibecheck(char **line)
{
	if (line)
		*line = NULL;
}

int		get_next_line(int fd, char **line)
{
	static t_chr	s[1024];

	if (fd < 0 || BUFFER_SIZE < 1 || !line)
	{
		vibecheck(line);
		return (-1);
	}
	if (!s[fd].str)
	{
		if (!(s[fd].str = (char*)malloc(BUFFER_SIZE + 1)))
			return (-1);
		s[fd].ret = read(fd, s[fd].str, BUFFER_SIZE);
		s[fd].str[s[fd].ret] = 0;
	}
	while (s[fd].ret != -2 && !fs(s[fd].str, '\n') && s[fd].ret == BUFFER_SIZE)
	{
		if (!(s[fd].t1 = (char*)malloc(BUFFER_SIZE + 1)))
			return (f1(&s[fd].str, NULL, -1));
		s[fd].ret = read(fd, s[fd].t1, BUFFER_SIZE);
		s[fd].t1[s[fd].ret] = 0;
		s[fd].t2 = s[fd].str;
		s[fd].str = ft_sjoin(s[fd].str, s[fd].t1);
		f1(&s[fd].t2, &s[fd].t1, 0);
	}
	return (s[fd].ret == -1 ? f1(&s[fd].str, &s[fd].t1, -1) : f(&s[fd], line));
}
