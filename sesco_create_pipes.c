/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_create_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:55:12 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/21 19:28:12 by isaadi           ###   ########.fr       */
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

/*
** This function gives g_program_return an exit status
** of the last executed program so we can use it
** with $?
*/

void	put_exit_status(int last)
{
	int		status;

	g_sig = 1;
	waitpid(g_pid, &status, 0);
	g_sig = 0;
	if (!g_parent && last)
	{
		if (WIFEXITED(status))
			g_program_return = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_program_return = WTERMSIG(status) + 128;
	}
}

/*
** Sesco: Here I simply open a certain amount
** of pipes then start executing the line of
** command/commands
*/

int		execute_non_builtins(t_cmd *data, int *pfd, int j)
{
	pid_t	tmp_pid;
	int		last;

	tmp_pid = -2;
	while (data)
	{
		last = 0;
		if (!is_builtin(data->find))
		{
			g_parent = 0;
			g_pid = fork();
			if (g_pid == -1)
				return (eerf(pfd) && !failing_error(data));
			if (g_pid == 0)
				execute_cmd(data, pfd, j);
			tmp_pid = g_pid;
			close(pfd[j - 1]);
			close(pfd[j]);
			last = 1;
		}
		j += 2;
		data = data->next;
	}
	tmp_pid != -2 ? put_exit_status(last) : 0;
	return (0);
}

/*
** This function opens a certain amount of pipes
** then initialize the holding array in reverse
** pipes_fd[1] = pipe[1];
** pipes_fd[2] = pipe[0];
*/

int		*open_pipes(t_cmd *data)
{
	int		*pipes_fd;
	int		i;
	int		j;
	int		pfd[2];

	g_cmds_length = 0;
	pipes_fd = count_cmds(data);
	i = g_cmds_length - 1;
	j = 1;
	pipes_fd[0] = dup(0);
	pipes_fd[(i * 2) + 1] = dup(1);
	while (i)
	{
		if (pipe(pfd) == -1)
		{
			free(pipes_fd);
			return ((int *)failing_error(data));
		}
		pipes_fd[j] = pfd[1];
		pipes_fd[j + 1] = pfd[0];
		j += 2;
		i--;
	}
	return (pipes_fd);
}

void	loop_in_data(void)
{
	t_cmd	*data;
	t_fnl	*tmp;
	int		*pfd;
	int		count;

	count = 0;
	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		if ((pfd = open_pipes(data)))
		{
			if (execute_builtins(data, pfd))
				return ;
			while (wait(NULL) != -1)
				;
			if (execute_non_builtins(data, pfd, 1))
				return ;
		}
		count++;
		tmp = tmp->next;
		free(pfd);
	}
}
