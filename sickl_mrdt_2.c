/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/21 19:35:42 by isaadi           ###   ########.fr       */
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

char	*spl_mst(char *msk, char *buf, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (is_ws(buf[i]) && c == NONLIT)
			msk[i] = WHTSPC;
		else
			msk[i] = c;
		i++;
	}
	return (msk);
}

void	e_multiline(void)
{
	if (g_mlt == '|')
		EPRINTS("minishell: multiline not supported");
	else
	{
		EPRINTS("minishell: unexpected EOF while looking for matching `");
		EPRINTC(g_mlt);
		EPRINTC('\'');
	}
}

void	init_sort_evar(void)
{
	t_evar	*term;
	t_evar	*path;
	t_evar	*pwd;
	t_evar	*shlvl;

	term = find_env_p("TERM");
	path = find_env_p("PATH");
	pwd = find_env_p("PWD");
	shlvl = find_env_p("SHLVL");
	continue_init_sort_evar(term, path, pwd, shlvl);
}

t_evar	*get_by_cardinal_pos(t_evar **tab, int pos)
{
	int		i;
	int		j;
	int		tar[4];

	i = -1;
	while (++i < 4)
		tar[i] = 0;
	i = -1;
	while (tab[++i])
	{
		j = i;
		while (tab[++j])
		{
			if (pos)
				(void)0;
		}
	}
	return (tab[0]);
}

void	continue_init_sort_evar
(t_evar *term, t_evar *path, t_evar *pwd, t_evar *shlvl)
{
	t_evar	**tab;
	int		i;
	t_evar	*smallest;

	tab = (t_evar*[]){return_non_null(term, path, pwd, shlvl),
	return_non_null(path, pwd, shlvl, NULL),
	return_non_null(pwd, shlvl, NULL, NULL),
	return_non_null(shlvl, NULL, NULL, NULL),
	NULL};
	i = 0;
	while (tab[i])
	{
		smallest = get_by_cardinal_pos(tab, i + 1);
		i++;
	}
}

t_evar	*return_non_null(t_evar *term, t_evar *path, t_evar *pwd, t_evar *shlvl)
{
	if (term)
		return (term);
	if (path)
		return (path);
	if (pwd)
		return (pwd);
	return (shlvl);
}
