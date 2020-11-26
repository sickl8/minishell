#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
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
** This function does two redirections or execute
** as a normal command no pipes are used
*/

void	execute_cmd(t_cmd *data, int prev_pipe)
{
	char	*arg[] = {"/usr/bin/ls", "-la", NULL};
	execve("/usr/bin/ls", arg, NULL);
}

void	redirect_to_pipe_one(t_cmd *data, int fd[2], int i, int prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid < -1)
		cleanup(EXIT);
	if (pid == 0)
	{
		if (data->next != NULL)
			execute_cmd(data, prev_pipe);
		else if (i == 1)
			dup2(fd[1], 1);
		else if (i != 1)
			dup2(prev_pipe, 0);
		execute_cmd(data, prev_pipe);
	}
	else
		wait(NULL);
}

/*
** This function redirects both input & output file descriptor
** for the commands in the middle
*/

void	redirect_to_pipe_two(t_cmd *data, int fd[2], int prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid < -1)
		cleanup(EXIT);
	if (pid == 0)
	{
		dup2(prev_pipe, 0);
		dup2(fd[1], 1);
		execute_cmd(data, prev_pipe);
	}
	else
		wait(NULL);
}

/*
** Most Of This Code Is Just To Make Piping Works
*/

void	loop_in_data_two(t_cmd *data)
{
	int		fd[2];
	int		prev_pipe;
	int		i;

	i = 1;
	prev_pipe = STDIN_FILENO;
	while (data)
	{
		if (pipe(fd) < 0)
			cleanup(EXIT);
		if (i != 1 && data->next != NULL)
			redirect_to_pipe_two(data, fd, prev_pipe);
		else
			redirect_to_pipe_one(data, fd, i, prev_pipe);
		close(fd[1]);
		close(prev_pipe);
		prev_pipe = fd[0];
		i++;
		data = data->next;
	}
	close(prev_pipe);
}

void	loop_in_data()
{
	t_cmd	*data;
	t_fnl	*tmp;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		loop_in_data_two(data);
		/*if (data->find)
		   printf("cmd - [%s]\n", data->find);
		if (tmp->path2exec)
		   printf("path:cmd - [%s]\n", tmp->path2exec);
		if (data->args)
			for (int i = 0; data->args[i]; i++)
				printf("args[%d] - [%s]\n", i, data->args[i]);
		if (data->redir)
			for (int i = 0; data->redir[i].file_name; i++)
				printf("type - [%d]; filename - [%s]\n", data->redir[i].type, data->redir[i].file_name);*/
		tmp = tmp->next;
	}
}
