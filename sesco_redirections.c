/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:03:33 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/09 17:03:49 by aamzouar         ###   ########.fr       */
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

void	open_redir_files(t_rdr *redir, mode_t mode, int index[2], int fd[2])
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
	mode_t	mode;

	index[0] = -1;
	index[1] = -1;
	fd[0] = -1;
	fd[1] = -1;
	mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	open_redir_files(redir, mode, index, fd);
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
