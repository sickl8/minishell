/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_bwrite.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 12:38:51 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:35:13 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header_def.h"
#include "header_proto.h"
#include "header_extern.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int		bflush(int fd)
{
	int	ret;

	ret = -2;
	if (g_bw.buf)
	{
		ret = write(fd, g_bw.buf[0][fd], g_bw.buf_i[0][fd]);
		free_and_set_to_null(&(g_bw.buf[0][fd]));
	}
	return (ret);
}

void	init_wbuf_fd(char **b, size_t *buf_i, int fd)
{
	if (!b[fd])
		if (!(MALLOC(&b[fd], BUFS + 1)))
			cleanup(EXIT);
	ft_memset(b[fd], '\0', BUFS + 1);
	buf_i[fd] = 0;
}

void	init_wbuf(char ***b, size_t **j)
{
	int	i;

	g_bw.buf = b;
	g_bw.buf_i = j;
	if (!(MALLOC(b, 8 * MAX_OPEN_FD + 8)))
		cleanup(EXIT);
	i = -1;
	while (++i <= MAX_OPEN_FD)
		b[0][i] = NULL;
	if (!(MALLOC(j, 8 * MAX_OPEN_FD + 8)))
		cleanup(EXIT);
	i = -1;
	while (++i <= MAX_OPEN_FD)
		j[0][i] = 0;
}

int		bwrite(int fd, void *buffer, size_t len)
{
	static size_t	*buf_i;
	static char		**buf;
	int				ret;

	if (len >= BUFS || fd < 0 || !buffer || fd > MAX_OPEN_FD)
		return (write(fd, buf, len));
	if (!buf)
		init_wbuf(&buf, &buf_i);
	if (!buf[fd])
		init_wbuf_fd(buf, buf_i, fd);
	if (buf_i[fd] + len <= BUFS)
	{
		WRITE2BUF(buf[fd] + buf_i[fd], buffer, len);
		buf_i[fd] += len;
	}
	else if (buf_i[fd] + len > BUFS)
	{
		ret = write(fd, buf[fd], buf_i[fd]);
		free_and_set_to_null(&buf[fd]);
		if (ret < 0 || (ret = write(fd, buffer, BUFS - buf_i[fd])) < 0)
			return (ret);
		init_wbuf_fd(buf, buf_i, fd);
	}
	return (len);
}
