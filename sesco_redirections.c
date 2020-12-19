/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:03:33 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/19 10:40:02 by aamzouar         ###   ########.fr       */
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

/*
** New Logic Always Create Files Before
** Needing To Redirect The Output Of Any
** Command
*/

void	create_files(t_rdr *redir)
{
	int		i;
	mode_t	mode;
	int		fd;

	mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	i = 0;
	while (redir[i].file_name)
	{
		fd = open(redir[i].file_name, O_CREAT, mode);
		close(fd);
		i++;
	}
}

void	open_redir_files(t_rdr *redir, int index[2], int fd[2])
{
	int		i;

	i = 0;
	while (redir[i].file_name)
	{
		if (redir[i].type == 0 || redir[i].type == 1)
		{
			index[1] = i;
			if (fd[1] != -1)
				close(fd[1]);
			if (redir[index[1]].type == 0)
				fd[1] = open(redir[index[1]].file_name, APPND);
			else if (redir[index[1]].type == 1)
				fd[1] = open(redir[index[1]].file_name, TRNCT);
		}
		else if (redir[i].type == 2)
		{
			index[0] = i;
			if (fd[0] != -1)
				close(fd[0]);
			fd[0] = open(redir[index[0]].file_name, O_RDONLY);
		}
		i++;
	}
}

void	make_a_redirection(t_rdr *redir)
{
	int		fd[2];
	int		index[2];

	index[0] = -1;
	index[1] = -1;
	fd[0] = -1;
	fd[1] = -1;
	open_redir_files(redir, index, fd);
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
