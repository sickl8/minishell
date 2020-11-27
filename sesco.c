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
	int		i;

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
** Find the command in $PATH
** also we check if the command is executable
*/

char	*find_in_path(char *tofind)
{
	t_evar			path;
	char			**paths;
	int				i;
	DIR				*dir;
	struct dirent	*file;
	struct stat		f_inf;
	char			*tmp;

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
** This function counts how many cmds
** are exist and also allocates enough
** memory to the holding array
*/

int		*count_cmds(t_cmd *data, int *i)
{
	int		*pipes_fd;

	while (data)
	{
		(*i)++;
		data = data->next;
	}
	if (!(MALLOC(pipes_fd, *i * 2)))
		cleanup(EXIT);
	return (pipes_fd);
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

	i = 0;
	pipes_fd = count_cmds(data, &i);
	j = 1;
	pipes_fd[0] = dup(0);
	pipes_fd[(i * 2) - 1] = dup(1);
	while (--i)
	{
		if (pipe(pfd) == -1)
		{
			free(pfd);
			cleanup(EXIT);
		}
		pipes_fd[j] = pfd[1];
		pipes_fd[j + 1] = pfd[0];
		j += 2;
	}
	return (pipes_fd);
}

/*
** here we check if it's a path or just a command
** path -> [/]
** command -> [no slash]
** also it redirects pipes
*/

void	execute_cmd(t_cmd *data, int *pfd, int j)
{
	dup2(pfd[j - 1], 0);
	close(pfd[j - 1]);
	dup2(pfd[j], 1);
	close(pfd[j]);
	if (ft_strchr(data->find, '/') != NULL)
	{
		execve(data->find, data->args, g_line->envp);
		PRINTS("minishell: ");
		PRINT(data->find);
		strerror(errno);
	}
	else
	{
		data->path2exec = find_in_path(data->find);
		execve(data->path2exec, data->args, g_line->envp);
		PRINTS("minishell: ");
		PRINT(data->find);
		PRINTS(": command not found");
	}
	PRINTS("\n");
	cleanup(RETURN);
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
		pid = fork();
		if (pid == -1)
		{
			free(pfd);
			cleanup(EXIT);
		}
		if (pid == 0)
			execute_cmd(data, pfd, j);
		else
		{
			wait(NULL);
			close(pfd[j - 1]);
			close(pfd[j]);
		}
		j += 2;
		data = data->next;
	}
	free(pfd);
}

void	loop_in_data()
{
	t_cmd	*data;
	t_fnl	*tmp;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		open_pipes_and_execute(data);
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
