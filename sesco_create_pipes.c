/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_create_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:55:12 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/18 19:26:46 by aamzouar         ###   ########.fr       */
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
	int		w_ret;

	w_ret = 1;
	while (w_ret > 0)
	{
		g_sig = 1;
		w_ret = waitpid(0, &status, 0);
		g_sig = 0;
		if (g_program_return != 1 && WIFEXITED(status))
			g_program_return = WEXITSTATUS(status);
		else if (g_program_return != 1 && WIFSIGNALED(status))
			g_program_return = WTERMSIG(status) + 128;
	}
}

void	parent_stuff(t_cmd *data)
{
	g_program_return = 0;
	if (!CMP(data->find, "cd"))
		bc_cd(data);
	else if (!CMP(data->find, "export"))
		bc_export(data);
	else if (!CMP(data->find, "unset"))
		bc_unset(data);
	else if (!CMP(data->find, "exit"))
	{
		// cleanup(RETURN);
		// exit(0);
		bc_exit(data->args);
	}
}

/*
** Sesco: Here I simply open a certain amount
** of pipes then start executing the line of
** command/commands
*/

void	open_pipes_and_execute(t_cmd *data, int *pfd)
{
	int		j;

	j = 1;
	while (data)
	{
		if (data->find)
		{
			g_pid = fork();
			if (g_pid == -1)
			{
				free(pfd);
				cleanup(EXIT);
			}
			if (g_pid == 0)
				execute_cmd(data, pfd, j);
			close(pfd[j - 1]);
			close(pfd[j]);
			parent_stuff(data);	
		}
		j += 2;
		data = data->next;
	}
	put_exit_status();
}

void	loop_in_data(void)
{
	t_cmd	*data;
	t_fnl	*tmp;
	int		*pfd;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		pfd = open_pipes(data);
		open_pipes_and_execute(data, pfd);
		tmp = tmp->next;
		free(pfd);
	}
}
