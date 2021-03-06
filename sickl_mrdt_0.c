/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/23 19:20:20 by isaadi           ###   ########.fr       */
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

void	copy_valid_chars(char *dst, char *src, char *msk, size_t tr_ln)
{
	size_t	i;

	i = 0;
	while (src[i] && i < tr_ln)
	{
		while (src[i] && msk[i] != LITERAL && msk[i] != SEMILIT &&
		msk[i] != NONLIT && msk[i] != '<' && msk[i] != '>' && msk[i] != '2')
		{
			src++;
			msk++;
		}
		if (!src[i])
			break ;
		dst[i] = src[i];
		i++;
	}
}

size_t	true_len(t_bm p)
{
	size_t	ret;

	if ((!(NCMP(p.buf, p.msk, p.cnt)) && (IS_REDIR(p.msk[0]))) ||
	(p.cnt == 2 && !NCMP(p.msk, "22", 2) && !NCMP(p.buf, ">>", 2)))
		return (p.cnt);
	ret = 0;
	while (p.buf[0] && p.msk[0] != WHTSPC && p.msk[0] != '>' && p.msk[0] != '<'
	&& p.msk[0] != '2')
	{
		if (p.msk[0] == SEMILIT || p.msk[0] == LITERAL || p.msk[0] == NONLIT)
			ret++;
		p.buf++;
		p.msk++;
	}
	return (ret);
}

t_bm	previous_word(t_bm rd, t_bm ref)
{
	while (ref.msk < (rd.msk - 1) && rd.msk[-1] == WHTSPC)
	{
		rd.msk--;
		rd.buf--;
	}
	if (ref.msk >= rd.msk || rd.msk[-1] == WHTSPC)
	{
		rd.msk = NULL;
		return (rd);
	}
	if (BASHSYN(rd.msk[-1]))
	{
		rd.cnt = 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
		return (rd);
	}
	else if (rd.msk[-1] == '2')
	{
		rd.cnt = 2 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
		return (rd);
	}
	rd.cnt = 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
	while (ref.msk < rd.msk && rd.msk[-1] != WHTSPC &&
	!BASHSYN(rd.msk[-1]))
		rd.cnt += 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
	return (rd);
}

void	exec_from_av(int ac, char **av)
{
	(void)ac;
	g_dashc = 1;
	set_pwd();
	ft_memset(g_bash_error, '\0', ARG_MAX + 2);
	if (ac == 2)
	{
		EPRINT("minishell: -c: option requires an argument\n");
		cleanup(2);
	}
	g_line->rd.buf = ft_strdup(av[2]);
	if (!g_line->rd.buf)
		cleanup(EXIT);
	if (format_string())
		bash_error();
	free_and_set_to_null(&g_line->rd.buf);
	cleanup(0);
	exit(g_program_return);
}

void	set_pwd(void)
{
	t_cmd	data;
	char	*pwd;
	char	*join;

	if (!(pwd = getcwd(NULL, 0)))
		cleanup(EXIT);
	join = ft_strjoin("PWD=", pwd);
	if (!join)
		eerf(pwd) && cleanup(EXIT);
	g_cmds_length = 1;
	data.args = (char*[]){ "export", join, NULL};
	bc_export_bk(&data);
	free(pwd);
	free(join);
	g_cmds_length = 0;
}
