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

void	parent_stuff(t_cmd *data)
{
	if (CMP(data->find, "exit") && !data->next)
		g_program_return = 0;
	if (!CMP(data->find, "cd") && g_cmds_length == 1)
		assign(&g_parent, 1, 4) && bc_cd(data);
	else if (!CMP(data->find, "export"))
		assign(&g_parent, 1, 4) &&
		(!data->args[1] ? 0 : bc_export_bk(data));
	else if (!CMP(data->find, "unset"))
		assign(&g_parent, 1, 4) && bc_unset(data);
	else if (!CMP(data->find, "exit"))
		assign(&g_parent, 1, 4) && bc_exit(data->args, data);
}

void	execute_builtins_continue(t_cmd *data, int *pfd, int cmd, int j)
{
	int		bk[2];

	bk[0] = dup(0);
	bk[1] = dup(1);
	dup2(pfd[j - 1], 0);
	close(pfd[j - 1]);
	dup2(pfd[j], 1);
	close(pfd[j]);
	make_a_redirection(data->redir);
	builtin(data, cmd);
}

int		execute_builtins(t_cmd *data, int *pfd, int *i)
{
	int		j;
	int		cmd;

	j = 1;
	*i = 0;
	while (data)
	{
		if (data->find && (cmd = is_builtin(data->find))) // maybe you shouldn't do data->find
		{
			g_parent = 0;
			g_pid = fork();
			g_pid_group[*i] = g_pid;
			if (g_pid == -1)
				return (eerf(pfd) && eerf(g_pid_group) && !failing_error(data));
			if (g_pid == 0)
				execute_builtins_continue(data, pfd, cmd, j);
			close(pfd[j - 1]);
			close(pfd[j]);
			parent_stuff(data);
			(*i)++;
		}
		j += 2;
		data = data->next;
	}
	return (0);
}

/*
** This function gives g_program_return an exit status
** of the last executed program so we can use it
** with $?
*/

void	put_exit_status(t_cmd *data)
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
	if (!g_parent && !data->next)
	{
		if (g_program_return != 1 && WIFEXITED(status))
			g_program_return = WEXITSTATUS(status);
		else if (g_program_return != 1 && WIFSIGNALED(status))
			g_program_return = WTERMSIG(status) + 128;
	}
	free(g_pid_group);
}

/*
** Sesco: Here I simply open a certain amount
** of pipes then start executing the line of
** command/commands
*/

int		execute_non_builtins(t_cmd *data, int *pfd, int i)
{
	int		j;

	j = 1;
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
		j += 2;
		i++;
		data = data->next;
	}
	put_exit_status(data);
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
	while (i--)
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

void	loop_in_data(void)
{
	t_cmd	*data;
	t_fnl	*tmp;
	int		*pfd;
	int		count;
	int		i;

	count = 0;
	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		malloc_pid_buffer(data);
		if ((pfd = open_pipes(data)))
		{
			if (execute_builtins(data, pfd, &i))
				return ;
			if (execute_non_builtins(data, pfd, i))
				return ;
		}
		count++;
		tmp = tmp->next;
		free(pfd);
	}
}
