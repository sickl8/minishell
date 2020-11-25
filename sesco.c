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

void	redirect_to_pipe_one(t_cmd *data, int fd[2], int i)
{
	if (fork() == 0)
	{
		if (data->next != NULL)
		{
		}
		else if (i == 1)
			dup2(fd[1], 1);
		else if (i != 1)
			dup2(fd[0], 0);
	}
	else
		wait(NULL);
}

/*
** This function redirects both input & output file descriptor
** for the commands in the middle
*/

void	redirect_to_pipe_two(t_cmd *data, int fd[2])
{
	if (fork() == 0)
	{
		dup2(fd[0], 0);
		dup2(fd[1], 1);
		// execute commands here
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
	int		i;

	pipe(fd);
	i = 1;
	while (data)
	{
		if (i != 1 && data->next != NULL)
			redirect_to_pipe_two(data, fd);
		else
			redirect_to_pipe_one(data, fd, i);
		i++;
		data = data->next;
	}
	close(fd[0]);
	close(fd[1]);
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
