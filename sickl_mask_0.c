/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mask_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:28 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/20 19:28:18 by isaadi           ###   ########.fr       */
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

void	bash_syntax(size_t *ref)
{
	size_t	i;

	i = *ref;
	g_line->rd.msk[i] = g_line->rd.buf[i];
	if (g_line->rd.buf[i] == '>' && g_line->rd.buf[i + 1] == '>')
	{
		g_line->rd.msk[i] = '2';
		i++;
		g_line->rd.msk[i] = '2';
	}
	*ref = i;
}

void	squote(size_t *ref)
{
	size_t	i;

	i = *ref;
	g_line->rd.msk[i] = '\'';
	while (g_line->rd.buf[++i] && g_line->rd.buf[i] != '\'')
		g_line->rd.msk[i] = LITERAL;
	if (g_line->rd.buf[i])
		g_line->rd.msk[i] = '\'';
	else
		i--;
	*ref = i;
}

void	dquote(size_t *ref)
{
	size_t	i;

	i = *ref;
	g_line->rd.msk[i] = '"';
	while (g_line->rd.buf[++i] && g_line->rd.buf[i] != '"')
	{
		if (g_line->rd.buf[i] == '\\')
			backslash(&i, SEMILIT);
		else if (g_line->rd.buf[i] == '$')
			env_var(&i);
		else
			g_line->rd.msk[i] = SEMILIT;
	}
	if (g_line->rd.buf[i])
		g_line->rd.msk[i] = '"';
	else
		i--;
	*ref = i;
}

void	backslash(size_t *ref, char lit_type)
{
	g_line->rd.msk[*ref] = '\\';
	if (lit_type == NONLIT)
	{
		if (g_line->rd.buf[*ref + 1])
			g_line->rd.msk[++(*ref)] = LITERAL;
	}
	else if (lit_type == SEMILIT)
	{
		if (g_line->rd.buf[*ref + 1])
		{
			if (g_line->rd.buf[++(*ref)] == '"' ||
			g_line->rd.buf[*ref] == '\\' ||
			g_line->rd.buf[*ref] == '$')
				g_line->rd.msk[*ref] = LITERAL;
			else
				assign(&(g_line->rd.msk[*ref - 1]), SEMILIT, 1) &&
				assign(&(g_line->rd.msk[*ref]), SEMILIT, 1);
		}
	}
}

int		set_mask(void)
{
	size_t	i;

	i = -1;
	memset(g_line->rd.msk, '\0', g_line->rd_len + 1);
	while (g_line->rd.buf[++i])
	{
		if (g_line->rd.buf[i] == '\\')
			backslash(&i, NONLIT);
		else if (g_line->rd.buf[i] == '"')
			dquote(&i);
		else if (g_line->rd.buf[i] == '\'')
			squote(&i);
		else if (g_line->rd.buf[i] == '|' || g_line->rd.buf[i] == '>' ||
		g_line->rd.buf[i] == '<' || g_line->rd.buf[i] == ';')
			bash_syntax(&i);
		else if (g_line->rd.buf[i] == '$')
			env_var(&i);
		else if (is_ws(g_line->rd.buf[i]))
			g_line->rd.msk[i] = WHTSPC;
		else
			g_line->rd.msk[i] = NONLIT;
	}
	return (0);
}
