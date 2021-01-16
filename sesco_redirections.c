/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:03:33 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_extern.h"
#include "header_errors.h"

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
				fd[1] = open(redir[i].file_name, O_WRONLY | O_APPEND | O_CREAT, PERM);
			 else if (redir[i].type == RR)
				fd[1] = open(redir[i].file_name, O_WRONLY | O_TRUNC | O_CREAT, PERM);
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
