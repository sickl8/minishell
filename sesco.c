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

void	print_data()
{
	t_cmd	*tmp;
	t_fnl	*test;
	
	test = g_list_of_commands;
	while (test)
	{
		tmp = test->cmd_and_args;
		while (tmp)
		{
			if (tmp->find)
			   printf("cmd - [%s]\n", tmp->find);
			/*if (tmp->path2exec)
			   printf("path:cmd - [%s]\n", tmp->path2exec);*/
			if (tmp->args)
				for (int i = 0; tmp->args[i]; i++)
					printf("args[%d] - [%s]\n", i, tmp->args[i]);
			if (tmp->redir)
				for (int i = 0; tmp->redir[i].file_name; i++)
					printf("type - [%d]; filename - [%s]\n", tmp->redir[i].type, tmp->redir[i].file_name);
			tmp = tmp->next;
		}
		test = test->next;
	}
}
