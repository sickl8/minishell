/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_split_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:02:38 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/20 19:29:01 by isaadi           ###   ########.fr       */
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

int		count_quote(char c)
{
	int		ret;
	char	*p;

	ret = 0;
	p = ft_strchr(g_line->rd.msk, c);
	while (p)
	{
		ret++;
		p = ft_strchr(++p, c);
	}
	return (ret);
}

#define H printf("%s:%d\n", __FILE__, __LINE__)

int		check_multiline(void)
{
	char	*p;

	p = ft_strrchr(g_line->rd.msk, '|');
	if (p)
		while (*(++p))
			if (*p != WHTSPC)
				break ;
	if (count_quote('\'') % 2 || count_quote('"') % 2 || (g_line->rd_len > 0 &&
	g_line->rd.buf[g_line->rd_len - 1] == '\\' &&
	g_line->rd.msk[g_line->rd_len - 1] == '\\') || (p && !*p))
	{
		g_bash_errno = E_MULTILINE;
		g_program_return = 2;
		if (count_quote('\'') % 2)
			g_mlt = '\'';
		else if (count_quote('"') % 2)
			g_mlt = '"';
		else if (g_line->rd_len > 0 &&
		g_line->rd.buf[g_line->rd_len - 1] == '\\' &&
		g_line->rd.msk[g_line->rd_len - 1] == '\\' && g_dashc && !(GPR = 0))
			return (0 * (g_bash_errno = 0));
		else if (p && !*p)
			g_mlt = '|';
		return (1);
	}
	return (0);
}

void	continue_get_cmd_p_args(char **ret, t_bm *redir, t_rdr **rdr)
{
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (redir[++i].buf)
	{
		if (STR_IS_REDIR(redir[i]))
		{
			rdr[0][j].file_name = redir[i + 1].buf;
			if (STR_IS_RL(redir[i]))
				rdr[0][j].type = RL;
			else if (STR_IS_RR(redir[i]))
				rdr[0][j].type = RR;
			else
				rdr[0][j].type = RRR;
			j++;
			i++;
		}
		else
			ret[i - 2 * j] = redir[i].buf;
	}
}

char	**get_cmd_p_args(t_bm *redir, t_rdr **rdr)
{
	char	**ret;
	int		cnt;
	int		total;

	cnt = 0;
	total = -1;
	while (redir[++total].buf)
		if (STR_IS_REDIR(redir[total]))
			cnt++;
	if (!(MALLOC(&(ret), 8 * (total - 2 * cnt + 1))) ||
	!(MALLOC(&(*rdr), 16 * (cnt + 1))))
		cleanup(EXIT);
	ret[total - 2 * cnt] = NULL;
	rdr[0][cnt].file_name = NULL;
	continue_get_cmd_p_args(ret, redir, rdr);
	return (ret);
}

void	set_cmd_2_null(t_cmd *t)
{
	t->cpa = NULL;
	t->find = NULL;
	t->args = NULL;
	t->path2exec = NULL;
}
