/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_init_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 14:33:06 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:41:04 by isaadi           ###   ########.fr       */
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

void	init_env(void)
{
	int		i;

	if (!g_line->envp)
		cleanup(EXIT);
	i = 0;
	while (g_line->envp[i])
		i++;
	if (!(MALLOC(&(g_line->env_var), sizeof(*(g_line->env_var)) * (i + 1))))
		cleanup(EXIT);
	i = -1;
	while (g_line->envp[++i])
	{
		g_line->env_var[i].name = NULL;
		g_line->env_var[i].value = NULL;
	}
	g_line->env_var[i].name = NULL;
	g_line->env_var[i].name_len = -1;
	g_line->env_var[i].value_len = -1;
	if (!(MALLOC(&(g_line->env_var[i].value), 1)))
		cleanup(EXIT);
	g_line->env_var[i].value[0] = '\0';
	continue_init_env();
}

void	init_envp(char **envp)
{
	int		i;
	int		len;

	if (!envp)
		cleanup(EXIT);
	i = 0;
	while (envp[i])
		i++;
	if (!(MALLOC(&(g_line->envp), 8 * i + 8)))
		cleanup(EXIT);
	g_line->envp[i] = NULL;
	i = -1;
	while (envp[++i])
	{
		len = ft_strlen(envp[i]);
		g_line->envp[i] = NULL;
		if (!(MALLOC(&(g_line->envp[i]), len + 1)))
			cleanup(EXIT);
		ft_strncpy(g_line->envp[i], envp[i], len + 1);
	}
}

void	init_buf(void)
{
	if (!(MALLOC(&(g_bash_error), ARG_MAX + 2)))
		exit(1);
}

void	init_line(void)
{
	g_line->rd.buf = NULL;
	g_line->rd.msk = NULL;
	g_line->env_var = NULL;
	g_line->env.buf = NULL;
	g_line->env.msk = NULL;
	g_line->scol = NULL;
	g_line->pipe = NULL;
	g_line->redir = NULL;
	g_line->envp = NULL;
	g_line->fd = NULL;
}

void	init_globals(t_line *ref)
{
	g_line = ref;
	g_bw.buf = NULL;
	g_bw.buf_i = NULL;
	g_sig = 0;
	g_builtin_errno = 0;
	g_bash_errno = 0;
	g_bash_commandid = 0;
	g_bash_error = NULL;
}
