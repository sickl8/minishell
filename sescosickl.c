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
extern char		**g_bash_command;

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
		}
		closedir(dir);
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

int		cd(t_cmd *data)
{
	return (0);
}

int		export(t_cmd *data)
{
	return (0);
}

int		env(void)
{
	int		i;

	i = 0;
	while (g_line->env_var[i].name)
	{
		PRINT(g_line->env_var[i].name);
		PRINTS("=");
		PRINT(g_line->env_var[i].value);
		i++;
	}
	return (0);
}

int		echo(t_cmd *data)
{
	int		i;
	int		pn;

	pn = 1;
	i = 1;
	if (data->args[i] && !CMP(data->args[i], "-n"))
	{
		pn = 0;
		i++;
	}
	while (data->args[i] && data->args[i + 1])
	{
		PRINT(data->args[i]);
		i++;
		PRINTS(" ");
	}
	if (data->args[i])
		PRINT(data->args[i]);
	if (pn)
		PRINTS("\n");
	return (0);
}

int		pwd(t_cmd *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	PRINT(cwd);
	PRINTS("\n");
	return (0);
}

int		unset(t_cmd *data)
{
	return (0);
}

int		builtin(t_cmd *data, int cmd)
{
	int		ret;

	if (cmd == BC_CD)
		ret = cd(data);
	else if (cmd == BC_ECHO)
		ret = echo(data);
	else if (cmd == BC_ENV)
		ret = env();
	else if (cmd == BC_EXIT)
	{
		cleanup(RETURN);
		exit(0);
	}
	else if (cmd == BC_EXPORT)
		ret = export(data);
	else if (cmd == BC_PWD)
		ret = pwd(data);
	else if (cmd == BC_UNSET)
		ret = unset(data);
	if (!ret)
		exit(0);
	return (ret);
}

int		is_builtin(char *str)
{
	int		i;

	i = 0;
	while (g_bash_command[++i])
	{
		if (!CMP(str, g_bash_command[i]))
			return (i);
	}
	return (0);
}

int		execute_cmd(t_cmd *data)
{
	int		cmd;

	cmd = is_builtin(data->find); 
	if (cmd)
		return (builtin(data, cmd));
	else if (ft_strchr(data->find, '/') != NULL)
		return (execve(data->find, data->args, g_line->envp));
	data->path2exec = find_in_path(data->find);
	if (!data->path2exec)
	{
		g_bash_errno = E_COMMAND;
		g_bash_error = data->path2exec;
		exit(127);
	}
	return (execve(data->path2exec, data->args, g_line->envp));
}

void	loop_in_pipe(t_cmd *data)
{
	t_cmd	*tmp;
	int		*fd;
	int		dfd[2];
	int		i;
	int		j;
	int		stat_loc;
	pid_t	pid;

	i = 0;
	tmp = data;
	while (data)
	{
		i++;
		data = data->next;
	}
	if (!(MALLOC(fd, i * 2)))
		cleanup(EXIT);
	fd[0] = dup(STDIN_FILENO);
	fd[i * 2 - 1] = dup(STDOUT_FILENO);
	j = 0;
	while (j < i - 1)
	{
		if (pipe(dfd) < 0)
			cleanup(EXIT);
		fd[1 + j * 2] = dfd[1];
		fd[2 + j * 2] = dfd[0];
		j++;
	}
	i = 0;
	data = tmp;
	while (data)
	{
		pid = fork();
		if (pid == -1)
			cleanup(EXIT);
		if (!pid)
		{
			if (dup2(fd[i] , 0) < 0)
				cleanup(EXIT);
			if (dup2(fd[i + 1] , 1) < 0)
				cleanup(EXIT);
			if (close(fd[i]) < 0)
				cleanup(EXIT);
			if (close(fd[i + 1]) < 0)
				cleanup(EXIT);
			fprintf(stderr, "returned %d\n", execute_cmd(data));
			fprintf(stderr, "did not exit\n");
		}
		else
		{
			wait(&stat_loc);
			if (!CMP(data->find, "cd"))
			{
				if (chdir(data->args[1] ? data->args[1] : find_env("HOME").value) < 0)
				{
					g_bash_error = data->args[1] ? data->args[1] : find_env("HOME").value;
					g_bash_errno = E_ERRNO;
					bash_error();
				}
			}
			if (close(fd[i]) < 0)
				cleanup(EXIT);
			if (close(fd[i + 1]) < 0)
				cleanup(EXIT);
		}
		i += 2;	
		data = data->next;
	}
}

void	loop_in_cmd()
{
	t_cmd	*data;
	t_fnl	*tmp;

	tmp = g_list_of_commands;
	while (tmp)
	{
		data = tmp->cmd_and_args;
		loop_in_pipe(data);
		tmp = tmp->next;
	}
}
