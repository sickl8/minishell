/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_get_next_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:36:55 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:36:56 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header_get_next_line.h"
#include "header_def.h"
#include "header_proto.h"
#include "libft/libft.h"

#include <stdio.h>

int		error(void *a, void *b, void *c)
{
	free(a);
	free(b);
	free(c);
	return (-1);
}

int		get_next_line(char **line)
{
	char	*ret;
	char	*join;
	long	rd_ret;
	char	*tmp;

	if (!line || BUFFER_SIZE < 1 || !(MALLOC(&(ret), BUFFER_SIZE + 1)))
		return (-1);
	if ((rd_ret = read(STDIN_FILENO, ret, BUFFER_SIZE)) < 0)
		return (error(ret, NULL, NULL));
	ret[rd_ret] = '\0';
	while (rd_ret == BUFFER_SIZE)
	{
		if (!(MALLOC(&(join), BUFFER_SIZE + 1)))
			return (error(ret, NULL, NULL));
		if ((rd_ret = read(STDIN_FILENO, join, BUFFER_SIZE)) < 0)
			return (error(ret, join, NULL));
		join[rd_ret] = '\0';
		tmp = ret;
		if (!(ret = ft_strjoin(ret, join)))
			return (error(tmp, join, NULL));
		error(tmp, join, NULL);
	}
	*line = ret;
	return (ft_strlen(ret));
}
