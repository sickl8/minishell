/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mask_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:34 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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

void	env_var(size_t *ref)
{
	size_t	i;

	i = *ref;
	g_line->rd.msk[i] = '$';
	if (g_line->rd.buf[i + 1] && g_line->rd.buf[i + 1] == '?')
	{
		g_line->rd.msk[i + 1] = ENVVAR;
		i += 2;
	}
	else if (g_line->rd.buf[i + 1] && c_env_var_comp(g_line->rd.buf[i + 1]) &&
	!(g_line->rd.buf[i + 1] >= '0' && g_line->rd.buf[i + 1] <= '9'))
	{
		*ref = ++i;
		g_line->rd.msk[i] = ENVVAR;
		while (g_line->rd.buf[++i] && c_env_var_comp(g_line->rd.buf[i]) &&
		i - *ref < ENV_NAME_LEN_MAX)
			g_line->rd.msk[i] = ENVVAR;
	}
	else if (!(c_env_var_comp(g_line->rd.buf[++i]) &&
	!(g_line->rd.buf[i] >= '0' && g_line->rd.buf[i] <= '9')))
		g_line->rd.msk[i - 1] = LITERAL;
	i--;
	*ref = i;
}

void	rplc_env_var()
{
	size_t	tlen;
	size_t	i;

	i = -1;
	tlen = 0;
	while (g_line->rd.buf[++i])
	{
		if (g_line->rd.msk[i] == '$')
			tlen += find_env_in_line(&i).value_len;
		else
			tlen++;
	}
	if (!(MALLOC(&(g_line->env.buf), tlen + 1)))
		cleanup(EXIT);
	if (!(MALLOC(&(g_line->env.msk), tlen + 2)))
		cleanup(EXIT);
	ft_memset(g_line->env.buf, 0, tlen + 1);
	ft_memset(g_line->env.msk, 0, tlen + 2);
	continue_rplc_env_var();
}

int		is_ws(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' ||
	c == '\v' || c == '\f' || c == '\r' || c == '\0')
		return (1);
	return (0);
}

int		env_var_comp(char *s)
{
	if (c_env_var_comp(*s) && (*s > '9' || *s < '0'))
	{
		s++;
		while (*s)
		{
			if (!c_env_var_comp(*s))
				return (0);
			s++;
		}
		return (1);
	}
	return (0);
}

int		c_env_var_comp(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
	(c >= 'a' && c <= 'z') || c == '_')
		return (1);
	return (0);
}