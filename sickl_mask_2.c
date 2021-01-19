/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mask_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:37 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/19 16:43:26 by isaadi           ###   ########.fr       */
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

void	continue_rplc_env_var(void)
{
	size_t	i;
	size_t	j;
	t_evar	env;
	char	escape;

	i = -1;
	j = 0;
	escape = NONLIT;
	while (g_line->rd.buf[++i])
	{
		escape = detect_escape(escape, g_line->rd.msk[i]);
		if (g_line->rd.msk[i] == '$')
		{
			env = find_env_in_line(&i);
			ft_strncpy(&g_line->env.buf[j], env.value, env.value_len);
			ft_memset(&g_line->env.msk[j], escape, env.value_len);
			j += env.value_len;
		}
		else
		{
			g_line->env.buf[j] = g_line->rd.buf[i];
			g_line->env.msk[j] = g_line->rd.msk[i];
			j++;
		}
	}
}

void	continue_split_wmask(t_bm *rd, t_bm **wr, size_t cnt, char c)
{
	size_t	i;
	char	*p;
	char	*bk;

	p = rd->msk;
	bk = rd->msk;
	i = -1;
	while (++i < cnt)
	{
		wr[0][i].msk = NULL;
		wr[0][i].buf = NULL;
		if (!(p = ft_strchr(p, c)))
			p = ft_strchr(rd->msk, '\0');
		if (!(MALLOC(&(wr[0][i].buf), p - bk + 1)))
			cleanup(EXIT);
		wr[0][i].buf[p - bk] = '\0';
		if (!(MALLOC(&(wr[0][i].msk), p - bk + 1)))
			cleanup(EXIT);
		wr[0][i].msk[p - bk] = '\0';
		ft_strncpy(wr[0][i].buf, rd->buf + (bk - rd->msk), p - bk);
		ft_strncpy(wr[0][i].msk, rd->msk + (bk - rd->msk), p - bk);
		bk = ++p;
	}
}

void	split_wmask(t_bm *rd, t_bm **wr, char c)
{
	size_t	i;
	size_t	cnt;

	cnt = 1;
	i = 0;
	while (rd->buf[i])
	{
		if (rd->msk[i] == c)
			cnt++;
		i++;
	}
	if (!(MALLOC(&(*wr), 24 * cnt + 24)))
		cleanup(EXIT);
	wr[0][cnt].msk = NULL;
	wr[0][cnt].buf = NULL;
	continue_split_wmask(rd, wr, cnt, c);
	rd->cnt = cnt;
}
