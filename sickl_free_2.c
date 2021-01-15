/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_free_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:22 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:36:48 by isaadi           ###   ########.fr       */
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

void	free_buf_and_mask(t_bm s)
{
	free(s.buf);
	free(s.msk);
}

void	free_tmp(void)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (g_line->scol[++i].buf)
	{
		free_buf_and_mask(g_line->scol[i]);
		j = -1;
		while (g_line->pipe[i][++j].buf)
		{
			free_buf_and_mask(g_line->pipe[i][j]);
			k = -1;
			while (g_line->redir[i][j][++k].buf)
				free_buf_and_mask(g_line->redir[i][j][k]);
			free(g_line->redir[i][j]);
		}
		free(g_line->pipe[i]);
		free(g_line->redir[i]);
	}
	fastn();
}

int		cleanup(int ex)
{
	free(g_bash_error);
	free(g_line->rd.buf);
	free(g_line->rd.msk);
	free(g_line->env.buf);
	free(g_line->env.msk);
	if (g_line->env_var)
		free_envar();
	if (g_line->scol)
		free_scol();
	if (g_line->pipe)
		free_pipe();
	if (g_bw.buf[0])
		free_g_bw();
	if (g_line->redir)
		free_redir();
	if (g_line->envp)
		free_envp();
	if (g_list_of_commands)
		free_loc();
	if (ex)
		handle_error(ex);
	return (0);
}
