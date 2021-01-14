/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bwrite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 12:38:51 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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

#define	write2buf(dst, src, len) ft_strncpy(dst, src, len)

//////////////////////////////////////
// void	handle_error()
// {
// 	exit(1);
// }

// void	free_g_bw()
// {
// 	int	i;

// 	i = -1;
// 	while (++i < MAX_OPEN_FD)
// 		free(g_bw.buf[0][i]);
// 	free(g_bw.buf[0]);
// 	free(g_bw.buf_i[0]);
// }

// int		cleanup(int ex)
// {
// 	// free(g_line->rd_buf);
// 	// free(g_line->mask);
// 	// free(g_line->env.buf);
// 	// free(g_line->env.msk);
// 	// if (g_line->env_var)
// 	// 	free_envar();
// 	// if (g_line->scol)
// 	// 	free_scol();
// 	// if (g_line->pipe)
// 	// 	free_pipe();
// 	// free(g_line->env_var);
// 	if (g_bw.buf[0])
// 		free_g_bw();
// 	if (ex)
// 		handle_error();
// 	return (0);
// }

// void	free_and_set_to_null(void *adr)
// {
// 	void	**cast;

// 	cast = (void**)adr;
// 	free(*cast);
// 	*cast = NULL;
// }

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

int		bwrite(int fd, void* buffer, size_t len)
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
		write2buf(buf[fd] + buf_i[fd], buffer, len);
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

////////////////////////////////////////////////////////////////////////////////

// void	init_globals()
// {
// 	g_bw.buf = NULL;
// 	g_bw.buf_i = NULL;
// }

// int		main()
// {
// 	init_globals();
// 	int fd = open("file", O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
// 	if (fd > 0)
// 		printf("%d was created and opened\n", fd);
// 	else
// 		printf("%d was not created and opened, errno = %d\n", fd, errno);
// 	BWR(0, "rawnasdfae\n");
// 	BWR(0, "rawnasdifnai fanaegn\n");
// 	BWR(0, "rawnafaef anwna \n");
// 	BWR(0, "rawnasdifnai fananwfnaegnagn \n");
// 	BWR(fd, "atwygfuaewfaiwefaemwfkmewfeigerggraoewgiuenrgnerg");
// 	BWR(fd, "atwygfuaewfaiwe\nfaemwfkmewfeigerggraoewgiuenrgnerg");
// 	BWR(fd, "atwygfuaewfaiwefaemwfkmewfeigerggraoewgiuenrgnerg");
// 	BWR(fd, "atwygfuaewfaiwefaemwfkmewfeigerggr\naoewgiuenrgnerg");
// 	BWR(fd, "atwygfuaewfaiwefaemwfkmewfeigerggra\noewgiuenrgnerg");
// 	BWR(fd, "atwygfuaewfaiwefa\nemwfkmewfeigerggraoewgiuenrgnerg");
// 	for (int i = 5; i > 0; i--)
// 	{
// 		printf("%d...\n", i);
// 		sleep(1);
// 	}
// 	bflush(0);
// 	for (int i = 5; i > 0; i--)
// 	{
// 		printf("%d...\n", i);
// 		sleep(1);
// 	}
// 	bflush(fd);
// 	cleanup(RETURN);
// 	return 0;
// }