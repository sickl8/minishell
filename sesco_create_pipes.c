/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_create_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:55:12 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/18 11:26:04 by isaadi           ###   ########.fr       */
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
	i = g_cmds_length;
	j = 1;
	pipes_fd[0] = dup(0);
	pipes_fd[(i * 2) - 1] = dup(1);
	while (--i)
	{
		if (pipe(pfd) == -1)
		{
			free(pipes_fd);
			return ((int *)failing_error(data));
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
	int		i;

	g_sig = 1;
	waitpid(g_pid, &status, 0);
	g_sig = 0;
	i = 0;
	while (g_pid_group[i + 1] != -2)
	{
		kill(g_pid_group[i], SIGKILL);
		waitpid(g_pid_group[i], NULL, 0);
		i++;
	}
	if (!g_parent)
	{
		if (g_program_return != 1 && WIFEXITED(status))
			g_program_return = WEXITSTATUS(status);
		else if (g_program_return != 1 && WIFSIGNALED(status))
			g_program_return = WTERMSIG(status) + 128;
	}
	free(g_pid_group);
}

void	parent_stuff(t_cmd *data)
{
	int		ret;
	char	*line;

	ret = 1;
	while (ret > 0)
		ret = get_next_line(&line) + 0 * eerf(line);
	if (CMP(data->find, "exit"))
		g_program_return = 0;
	if (!CMP(data->find, "cd") && g_cmds_length == 1)
		assign(&g_parent, 1, 4) && bc_cd(data);
	else if (!CMP(data->find, "export"))
		assign(&g_parent, 1, 4) &&
		(!data->args[1] ? 0 : bc_export_bk(data));
	else if (!CMP(data->find, "unset"))
		assign(&g_parent, 1, 4) && bc_unset(data);
	else if (!CMP(data->find, "exit"))
		assign(&g_parent, 1, 4) && bc_exit(data->args);
}

/*
** Sesco: Here I simply open a certain amount
** of pipes then start executing the line of
** command/commands
*/

int		open_pipes_and_execute(t_cmd *data, int *pfd)
{
	int		j;
	int		i;

	j = 1;
	i = 0;
	while (data)
	{
		g_parent = 0;
		g_pid = fork();
		g_pid_group[i] = g_pid;
		if (g_pid == -1)
			return (eerf(pfd) && eerf(g_pid_group) && !failing_error(data));
		if (g_pid == 0)
			execute_cmd(data, pfd, j);
		close(pfd[j - 1]);
		close(pfd[j]);
		if (data->find)
			parent_stuff(data);
		j += 2;
		i++;
		data = data->next;
	}
	put_exit_status();
	return (0);
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
		malloc_pid_buffer(data);
		if ((pfd = open_pipes(data)))
			if (open_pipes_and_execute(data, pfd))
				return ;
		count++;
		tmp = tmp->next;
		free(pfd);
	}
}
