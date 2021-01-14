/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_free_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:19 by isaadi            #+#    #+#             */
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

void	free_g_bw()
{
	int	i;

	i = -1;
	while (++i < MAX_OPEN_FD)
		free(g_bw.buf[0][i]);
	free(g_bw.buf[0]);
	free(g_bw.buf_i[0]);
}

void	free_pipe()
{
	int	i;
	int	j;

	i = -1;
	while (g_line->pipe[++i])
	{
		j = -1;
		while (g_line->pipe[i][++j].buf)
		{
			free(g_line->pipe[i][j].buf);
			free(g_line->pipe[i][j].msk);
		}
		free(g_line->pipe[i]);
	}
	free(g_line->pipe);
}

void	free_scol()
{
	int	i;

	i = -1;
	while (g_line->scol[++i].buf && g_line->scol[i].msk)
	{
		free(g_line->scol[i].buf);
		free(g_line->scol[i].msk);
	}
	free(g_line->scol);
}

void	free_envar(void)
{
	int	i;

	i = -1;
	while (g_line->env_var[++i].name_len != (size_t)-1 &&
	g_line->env_var[i].value_len != (size_t)-1)
	{
		free(g_line->env_var[i].name);
		free(g_line->env_var[i].value);
	}
	free(g_line->env_var[i].value);
	free(g_line->env_var);
}

void	free_it()
{
	int		i;

	if (g_line->it)
	{
		i = 0;
		while (g_line->it[i].buf)
		{
			free(g_line->it[i].buf);
			free(g_line->it[i].msk);
			i++;
		}
		free(g_line->it);
	}
	g_line->it = NULL;
}