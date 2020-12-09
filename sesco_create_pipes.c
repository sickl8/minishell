/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_create_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:55:12 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/09 17:23:44 by aamzouar         ###   ########.fr       */
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

	i = 0;
	pipes_fd = count_cmds(data, &i);
	j = 1;
	pipes_fd[0] = dup(0);
	pipes_fd[(i * 2) - 1] = dup(1);
	while (--i)
	{
		if (pipe(pfd) == -1)
		{
			free(pipes_fd);
			cleanup(EXIT);
		}
		pipes_fd[j] = pfd[1];
		pipes_fd[j + 1] = pfd[0];
		j += 2;
	}
	return (pipes_fd);
}

/*
** This function gives g_program_return an exit status
** of the last executed program so we can use it
** with $?
*/

void	put_exit_status(void)
{
	int		status;

	wait(&status);
	if (WIFEXITED(status))
		g_program_return = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_program_return = WTERMSIG(status);
}

void	parent_stuff(t_cmd *data, int *pfd, int j)
{
	put_exit_status();
	if (!CMP(data->find, "cd") && chdir(data->args[1]) < 0)
	{
		g_program_return = 1;
		g_bash_errno = E_ERRNO;
		ft_strncpy(g_bash_error, data->args[1], -1);
		g_bash_commandid = BC_CD;
		bash_error();
	}
	else if (!CMP(data->find, "export"))
		bc_export(data);
	else if (!CMP(data->find, "unset"))
		bc_unset(data);
	else if (!CMP(data->find, "exit"))
	{
		cleanup(RETURN);
		exit(0);
	}
	close(pfd[j - 1]);
	close(pfd[j]);
}

/*
** Sesco: Here I simply open a certain amount
** of pipes then start executing the line of
** command/commands
*/

void	open_pipes_and_execute(t_cmd *data)
{
	int		*pfd;
	int		j;
	pid_t	pid;

	j = 1;
	pfd = open_pipes(data);
	while (data)
	{
		if (data->find)
		{
			pid = fork();
			if (pid == -1)
			{
				free(pfd);
				cleanup(EXIT);
			}
			if (pid == 0)
				execute_cmd(data, pfd, j);
			else
				parent_stuff(data, pfd, j);
		}
		j += 2;
		data = data->next;
	}
	free(pfd);
}

void	loop_in_data(void)
{
	t_cmd	*data;
	t_fnl	*tmp;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		open_pipes_and_execute(data);
		tmp = tmp->next;
	}
}
