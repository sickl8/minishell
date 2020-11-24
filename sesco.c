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

void	loop_in_data_two(t_cmd *data, int check)
{
	int		fd[2];
	int		i;

	i = 1;
	pipe(fd);
	while (data)
	{
		if (g_reset_fd == 1)
			pipe(fd);
		if (g_reset_fd == 1 || data->next)
		i++;
		data = data->next;
	}
}

void	loop_in_data()
{
	t_cmd	*data;
	t_fnl	*tmp;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		if (data->next != NULL)
			loop_in_data_two(data, 1);
		else
			loop_in_data_two(data, 0);
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
