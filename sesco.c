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
** this function frees double pointer
*/

void	free_path(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

/*
** Adding a slash [/] to the end of a path
** if there's none
*/

char	*fix_path(char **paths, int i)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(paths[i]);
	if (paths[i][len - 1] != '/')
	{
		if (!(MALLOC(ret, len + 2)))
		{
			free_path(paths);
			cleanup(EXIT);
		}
		ft_strncpy(ret, paths[i], len);
		ret[len] = '/';
		ret[len + 1] = '\0';
		free(paths[i]);
		return (ret);
	}
	return (paths[i]);
}

/*
** Find command in $PATH
*/

char	*find_in_path(char *tofind)
{
	t_evar		path;
	char		**paths;
	int		i;
	DIR		*dir;
	struct dirent	*file;
	struct stat	f_inf;
	char		*tmp;

	i = 0;
	path = find_env("PATH");
	paths = ft_split(path.value, ':');
	while(paths[i])
	{
		dir = opendir(paths[i]);
		if (dir)
		{
			while ((file = readdir(dir)))
			{
				if (!CMP(tofind, file->d_name))
				{
					paths[i] = fix_path(paths, i);
					if (!(tmp = ft_strjoin(paths[i], file->d_name)))
					{
						free_path(paths);
						cleanup(EXIT);
					}
					if (stat(tmp,&f_inf) != -1)
					{
						if (f_inf.st_mode & S_IXUSR)
						{
							free_path(paths);
							closedir(dir);
							return (tmp);
						}
					
					}
					free(tmp);
				}
			}
			closedir(dir);
		}
		i++;
	}
	free_path(paths);
	return (NULL);
}

/*
** here we check if it's a path or just a command
** path -> [/]
** command -> [no slash]
*/

void	execute_cmd(t_cmd *data, int prev_pipe)
{
	if (ft_strchr(data->find, '/') != NULL)
		execve(data->find, data->args, g_line->envp);
	else
	{
		data->path2exec = find_in_path(data->find);
		execve(data->path2exec, data->args, g_line->envp);
	}
}

void	redirect_to_pipe_one(t_cmd *data, int fd[2], int i, int prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		cleanup(EXIT);
	if (pid == 0)
	{
		if (i == 1 && data->next == NULL)
			execute_cmd(data, prev_pipe);
		else if (i == 1)
			dup2(fd[1], 1);
		else if (i != 1)
			dup2(prev_pipe, 0);
		close(fd[1]);
		close(prev_pipe);
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
	if (pid == -1)
		cleanup(EXIT);
	if (pid == 0)
	{
		dup2(prev_pipe, 0);
		dup2(fd[1], 1);
		execute_cmd(data, prev_pipe);
		close(fd[1]);
		close(prev_pipe);
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
	int		bk[2];
	int		i;

	i = 1;
	prev_pipe = STDIN_FILENO;
	bk[0] = dup(0);
	bk[1] = dup(1);
	while (data)
	{
		if (pipe(fd) == -1)
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
	dup2(bk[0], 0);
	dup2(bk[1], 1);
	close(bk[0]);
	close(bk[1]);
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
