/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:03:33 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/19 10:52:45 by aamzouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "libft/libft.h"
#include "proto.h"
#include "typedef.h"
#include "def.h"
#include "global.h"
#include "errors.h"

#include <stdio.h>

void	open_redir_files(t_rdr *redir, int fd[2])
{
	int		i;

	i = 0;
	while (redir[i].file_name)
	{
		if (redir[i].type == RRR || redir[i].type == RR)
		{
			if (redir[i].type == RRR)
				fd[1] = open(redir[i].file_name, APPND | O_CREAT, PERM);
			else if (redir[i].type == RR)
				fd[1] = open(redir[i].file_name, TRNCT | O_CREAT, PERM);
		}
		else if (redir[i].type == RL)
			fd[0] = open(redir[i].file_name, O_RDONLY);
		if (fd[0] == -1 || fd[1] == -1)
		{
			g_bash_errno = E_ERRNO;
			ft_strcpy(g_bash_error, redir[i].file_name);
			g_bash_commandid = BC_DEF;
			bash_error();
			return ;
		}
		i++;
	}
}

void	make_a_redirection(t_rdr *redir)
{
	int		fd[2];

	fd[0] = -2;
	fd[1] = -2;
	open_redir_files(redir, fd);
	if (fd[0] == -1 || fd[1] == -1)
		exit(1);
	if (fd[0] > -1)
	{
		dup2(fd[0], 0);
		close(fd[0]);
	}
	if (fd[1] > -1)
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
}
