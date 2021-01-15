/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_split_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:02:41 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:47:38 by isaadi           ###   ########.fr       */
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

t_cmd	*get_cmd(t_bm **redir)
{
	t_cmd	*ret;
	t_cmd	**tracer;
	int		i;

	i = -1;
	tracer = &ret;
	while (redir[++i])
	{
		if (!(MALLOC(&(*tracer), 56)))
			cleanup(EXIT);
		set_cmd_2_null(*tracer);
		(*tracer)->next = NULL;
		(*tracer)->fd_read = 0;
		(*tracer)->fd_write = 1;
		(*tracer)->cpa = get_cmd_p_args(redir[i], &((*tracer)->redir));
		(*tracer)->find = (*tracer)->cpa[0];
		(*tracer)->args = (*tracer)->cpa;
		tracer = &(*tracer)->next;
	}
	return (ret);
}

int		initial_error_check(void)
{
	if (check_multiline() || check_syntax())
		return (0);
	return (1);
}

int		assign(void *p, unsigned long long v, int size)
{
	if (size == 1)
		*((char*)(p)) = (char)v;
	else if (size == 2)
		*((short*)(p)) = (short)v;
	else if (size == 4)
		*((int*)(p)) = (int)v;
	else if (size == 8)
		*((long*)(p)) = (long)v;
	return (1);
}

void	reset_stdin(int *bk)
{
	if (close(0) == -1)
		handle_error(1);
	if (dup2(*bk, 0) == -1)
		handle_error(1);
	if (close(*bk) == -1)
		handle_error(1);
	backup_stdin(bk);
}

void	backup_stdin(int *bk)
{
	if ((*bk = dup(STDIN_FILENO)) == -1)
		handle_error(1);
}
