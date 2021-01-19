/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_split_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:34:11 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/19 18:24:46 by isaadi           ###   ########.fr       */
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

void	exec(t_fnl **tracer, int i, int x)
{
	while (g_line->it[++x].buf)
	{
		g_line->rd = g_line->it[x];
		rplc_env_var();
		split_wmask(&g_line->env, &g_line->scol, ';');
		if (!(MALLOC(&(g_line->pipe), 8 * g_line->env.cnt + 8)))
			cleanup(EXIT);
		g_line->pipe[g_line->env.cnt] = NULL;
		split_pipe();
		split_redirects();
		i = -1;
		g_list_of_commands = NULL;
		tracer = &g_list_of_commands;
		while (++i < (int)g_line->env.cnt)
		{
			if (!(MALLOC(&(*tracer), 16)))
				cleanup(EXIT);
			(*tracer)->cmd_and_args = NULL;
			(*tracer)->next = NULL;
			(*tracer)->cmd_and_args = get_cmd(g_line->redir[i]);
			tracer = &(*tracer)->next;
		}
		loop_in_data();
		free_tmp();
	}
}

int		check_s_bashsyn(t_bm p)
{
	t_bm	tmp;

	tmp = p;
	tmp = previous_word(tmp, g_line->rd);
	if (!tmp.msk || tmp.msk[0] == ';' || tmp.msk[0] == '|')
	{
		g_bash_errno = E_SYNTAX;
		if (!tmp.msk)
			ft_strncpy(g_bash_error, p.msk, 1);
		else
			ft_strncpy(g_bash_error, tmp.msk, 1);
		return (1);
	}
	return (0);
}

int		check_s_redir(t_bm p)
{
	t_bm	tmp;

	tmp = p;
	tmp.buf += tmp.cnt;
	tmp.msk += tmp.cnt;
	tmp = next_word(tmp);
	if (!tmp.msk || STR_IS_REDIR(tmp) || BASHSYN(tmp.msk[0]))
	{
		g_bash_errno = E_SYNTAX;
		if (!tmp.msk)
			ft_strncpy(g_bash_error, "newline", 7);
		else
			ft_strncpy(g_bash_error, tmp.buf, tmp.cnt);
		return (1);
	}
	return (0);
}

int		check_syntax(void)
{
	t_bm	p;

	p = next_word(g_line->rd);
	while (p.msk)
	{
		if (STR_IS_REDIR(p) && check_s_redir(p))
		{
			g_program_return = E_SYNTAX;
			return (1);
		}
		else if (!STR_IS_REDIR(p) && BASHSYN(p.msk[0]) && check_s_bashsyn(p))
		{
			g_program_return = E_SYNTAX;
			return (1);
		}
		p.buf += p.cnt;
		p.msk += p.cnt;
		p = next_word(p);
	}
	return (0);
}

void	continue_split_redirects(t_bm *rd, t_bm **wr, size_t wc)
{
	t_bm	p;
	size_t	tr_ln;
	size_t	i;

	i = 0;
	p = next_word(*rd);
	while (i < wc)
	{
		wr[0][i].buf = NULL;
		wr[0][i].msk = NULL;
		tr_ln = true_len(p);
		if (!(MALLOC(&(wr[0][i].buf), tr_ln + 1)))
			cleanup(EXIT);
		wr[0][i].buf[tr_ln] = '\0';
		if (!(MALLOC(&(wr[0][i].msk), tr_ln + 1)))
			cleanup(EXIT);
		wr[0][i].msk[tr_ln] = '\0';
		copy_valid_chars(wr[0][i].buf, p.buf, p.msk, tr_ln);
		copy_valid_chars(wr[0][i].msk, p.msk, p.msk, tr_ln);
		p.msk += p.cnt;
		p.buf += p.cnt;
		p = next_word(p);
		i++;
	}
}
