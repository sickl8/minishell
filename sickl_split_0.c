/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_split_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:02:38 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/19 16:01:03 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_extern.h"
#include "header_errors.h"

#include "header_get_next_line.h"

#include <stdio.h>

t_bm	next_word(t_bm rd)
{
	while (rd.msk[0] == WHTSPC && rd.buf[0])
	{
		rd.msk++;
		rd.buf++;
	}
	if (BASHSYN(rd.msk[0]))
	{
		rd.cnt = 1;
		return (rd);
	}
	else if (rd.msk[0] == '2')
	{
		rd.cnt = 2;
		return (rd);
	}
	else if (!rd.buf[0])
	{
		rd.msk = NULL;
		return (rd);
	}
	rd.cnt = 0;
	while (rd.msk[rd.cnt] != WHTSPC &&
	!BASHSYN(rd.msk[rd.cnt]) && rd.msk[rd.cnt] != '2' && rd.buf[rd.cnt])
		rd.cnt++;
	return (rd);
}

size_t	count_words_redir(t_bm *rd)
{
	size_t	ret;
	t_bm	p;

	ret = 0;
	p = next_word(*rd);
	while (p.msk)
	{
		ret++;
		p.msk += p.cnt;
		p.buf += p.cnt;
		p = next_word(p);
	}
	return (ret);
}

void	split_redirect(t_bm *rd, t_bm **wr)
{
	size_t	wc;

	wc = count_words_redir(rd);
	if (!(MALLOC(&(*wr), 24 * wc + 24)))
		cleanup(EXIT);
	wr[0][wc].buf = NULL;
	wr[0][wc].msk = NULL;
	continue_split_redirects(rd, wr, wc);
}

void	split_redirects(void)
{
	size_t	i;
	size_t	j;

	if (!(MALLOC(&(g_line->redir), 8 * g_line->env.cnt + 8)))
		cleanup(EXIT);
	g_line->redir[g_line->env.cnt] = NULL;
	i = -1;
	while (++i < g_line->env.cnt)
	{
		g_line->redir[i] = NULL;
		if (!(MALLOC(&(g_line->redir[i]), 8 * g_line->scol[i].cnt + 8)))
			cleanup(EXIT);
		g_line->redir[i][g_line->scol[i].cnt] = NULL;
		j = -1;
		while (++j < g_line->scol[i].cnt)
		{
			g_line->redir[i][j] = NULL;
			split_redirect(&g_line->pipe[i][j], &g_line->redir[i][j]);
		}
	}
}

void	split_pipe(void)
{
	size_t	i;

	i = -1;
	while (++i < g_line->env.cnt)
	{
		g_line->pipe[i] = NULL;
		split_wmask(&g_line->scol[i], &g_line->pipe[i], '|');
	}
}
