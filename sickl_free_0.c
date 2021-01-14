/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_free_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:27:11 by isaadi            #+#    #+#             */
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

void	free_and_set_to_null(void *adr)
{
	void	**cast;

	cast = (void**)adr;
	free(*cast);
	*cast = NULL;
}

void	fastn()
{
	void	*tmp;

	free_and_set_to_null(&g_line->redir);
	free_and_set_to_null(&g_line->scol);
	free_and_set_to_null(&g_line->pipe);
	free_and_set_to_null(&g_line->env.buf);
	free_and_set_to_null(&g_line->env.msk);
	free_and_set_to_null(&g_line->rd.buf);
	free_and_set_to_null(&g_line->rd.msk);
	while (g_list_of_commands)
	{
		while (g_list_of_commands->cmd_and_args)
		{
			free(g_list_of_commands->cmd_and_args->cpa);
			free(g_list_of_commands->cmd_and_args->redir);
			free(g_list_of_commands->cmd_and_args->path2exec);
			tmp = g_list_of_commands->cmd_and_args;
			g_list_of_commands->cmd_and_args =
			g_list_of_commands->cmd_and_args->next;
			free(tmp);
		}
		tmp = g_list_of_commands;
		g_list_of_commands = g_list_of_commands->next;
		free(tmp);
	}
}

void	free_loc()
{
	void	*tmp;

	while (g_list_of_commands)
	{
		while (g_list_of_commands->cmd_and_args)
		{
			free(g_list_of_commands->cmd_and_args->cpa);
			free(g_list_of_commands->cmd_and_args->redir);
			tmp = g_list_of_commands->cmd_and_args;
			g_list_of_commands->cmd_and_args =
			g_list_of_commands->cmd_and_args->next;
			free(tmp);
		}
		tmp = g_list_of_commands;
		g_list_of_commands = g_list_of_commands->next;
		free(tmp);
	}
}

void	free_envp()
{
	int		i;

	i = -1;
	while (g_line->envp[++i])
		free(g_line->envp[i]);
	free(g_line->envp);
}

void	free_redir()
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = -1;
	while (g_line->redir[++i])
	{
		j = -1;
		while (g_line->redir[i][++j])
		{
			k = -1;
			while (g_line->redir[i][j][++k].buf)
				free_buf_and_mask(g_line->redir[i][j][k]);
			free(g_line->redir[i][j]);
		}
		free(g_line->redir[i]);
	}
	free(g_line->redir);
}