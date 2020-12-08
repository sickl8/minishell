/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 13:30:07 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/08 19:33:16 by aamzouar         ###   ########.fr       */
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
	while(paths && paths[i])
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
			free(pipes_fd);
			cleanup(EXIT);
		}
		pipes_fd[j] = pfd[1];
		pipes_fd[j + 1] = pfd[0];
		j += 2;
	}
	return (pipes_fd);
}

// this is sickl_'s code please review it




int		bc_cd(t_cmd *data)
{
	return (0);
}

void	ft_strcpy(char *dst, char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int		bc_export(t_cmd *data)
{
	int		i;
	int		len;
	t_evar	*tmp;
	char	**var_val;

	len = 0;
	while (g_line->env_var[len].name != NULL)
		len++;
	len += 2;
	if (!(MALLOC(tmp, len)))
		cleanup(EXIT);
	i = 0;
	while (i < (len - 2))
	{
		if (!(MALLOC(tmp[i].name, (ft_strlen(g_line->env_var[i].name) + 1))))
			cleanup(EXIT);
		ft_strcpy(tmp[i].name, g_line->env_var[i].name);
		if (!(MALLOC(tmp[i].value, (ft_strlen(g_line->env_var[i].value) + 1))))
			cleanup(EXIT);
		ft_strcpy(tmp[i].value, g_line->env_var[i].value);
		tmp[i].name_len = g_line->env_var[i].name_len;
		tmp[i].value_len = g_line->env_var[i].value_len;
		i++;
	}
	var_val = ft_split(data->args[1], '=');
	if (!(MALLOC(tmp[i].name, (ft_strlen(var_val[0]) + 1))))
		cleanup(EXIT);
	ft_strcpy(tmp[i].name, var_val[0]);
	if (!(MALLOC(tmp[i].value, (ft_strlen(var_val[1]) + 1))))
		cleanup(EXIT);
	ft_strcpy(tmp[i].value, var_val[1]);
	tmp[i].name_len = ft_strlen(var_val[0]);
	tmp[i].value_len = ft_strlen(var_val[1]);
	i++;
	tmp[i].name = NULL;
	tmp[i].value = NULL;
	tmp[i].name_len = -1;
	tmp[i].value_len = -1;
	free_path(var_val);
	free(g_line->env_var);
	g_line->env_var = tmp;
	return (0);
}

int		bc_env(void)
{
	int		i;

	i = 0;
	while (g_line->env_var[i].name)
	{
		PRINT(g_line->env_var[i].name);
		PRINTS("=");
		PRINT(g_line->env_var[i].value);
		PRINTS("\n");
		i++;
	}
	return (0);
}

int		bc_echo(t_cmd *data)
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
		PRINT("\n");
	return (0);
}

int		bc_pwd(t_cmd *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	PRINT(cwd);
	PRINTS("\n");
	return (0);
}

int		bc_unset(t_cmd *data)
{
	int		i;
	int		j;
	int		len;
	t_evar	*tmp;
	
	len = 0;
	while (g_line->env_var[len].name != NULL)
		len++;
	if (!(MALLOC(tmp, len)))
		cleanup(EXIT);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (CMP(g_line->env_var[i].name, data->args[1]))
		{
			if (!(MALLOC(tmp[j].name, (ft_strlen(g_line->env_var[i].name) + 1))))
				cleanup(EXIT);
			ft_strcpy(tmp[j].name, g_line->env_var[i].name);
			if (!(MALLOC(tmp[j].value, (ft_strlen(g_line->env_var[i].value) + 1))))
				cleanup(EXIT);
			ft_strcpy(tmp[j].value, g_line->env_var[i].value);
			tmp[j].name_len = g_line->env_var[i].name_len;
			tmp[j].value_len = g_line->env_var[i].value_len;
			j++;
		}
		i++;
	}
	tmp[j].name = NULL;
	tmp[j].value = NULL;
	tmp[j].name_len = -1;
	tmp[j].value_len = -1;
	free(g_line->env_var);
	g_line->env_var = tmp;
	return (0);
}

void	bc_program_return(void)
{
	PRINT("minishell: ");
	ft_putnbr_fd(g_program_return, 1);
	PRINT(": command not found\n");
	exit(127);
}

int		builtin(t_cmd *data, int cmd)
{
	int		ret;
	
	if (cmd == BC_CD)
		ret = bc_cd(data);
	else if (cmd == BC_ECHO)
		ret = bc_echo(data);
	else if (cmd == BC_ENV)
		ret = bc_env();
	else if (cmd == BC_EXIT)
	{
		cleanup(RETURN);
		exit(0);
	}
	else if (cmd == BC_EXPORT)
		ret = 0;
	else if (cmd == BC_PWD)
		ret = bc_pwd(data);
	else if (cmd == BC_UNSET)
		ret = 0;
	else if (cmd == 1337)
		bc_program_return();
	if (!ret)
		exit(0);
	return (ret);
}

int		is_builtin(char *str)
{
	int		i;

	i = 0;
	if (!CMP(str, "?"))
		return (1337);
	while (g_bash_command[++i])
	{
		if (!CMP(str, g_bash_command[i]))
			return (i);
	}
	return (0);
}

// end of the sickl_'s code

void	make_a_redirection(t_rdr *redir)
{
	int		i;
	int		fd[2];
	int		index[2];
	mode_t	mode;

	i = 0;
	index[0] = -1;
	index[1] = -1;
	fd[0] = -1;
	fd[1] = -1;
	mode = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	while (redir[i].file_name)
	{
		if (redir[i].type == 0 || redir[i].type == 1)
		{
			index[1] = i;
			if (fd[1] != -1)
				close(fd[1]);
			if (redir[index[1]].type == 0)
				fd[1] = open(redir[index[1]].file_name, O_WRONLY | O_CREAT | O_APPEND, mode);
			else if (redir[index[1]].type == 1)
				fd[1] = open(redir[index[1]].file_name, O_WRONLY | O_CREAT | O_TRUNC, mode);
		}
		else if (redir[i].type == 2)
		{
			index[0] = i;
			if (fd[0] != -1)
				close(fd[0]);
			fd[0] = open(redir[index[0]].file_name, O_RDONLY);
		}
		i++;
	}
	if (fd[0] > -1)
	{
		dup2(fd[0], 0);
		close(fd[0]);
	}
	if (fd[1] > -1)
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
}

/*
** here we check if it's a path or just a command
** path -> [/]
** command -> [no slash]
** also it redirects pipes
*/

void	execute_cmd(t_cmd *data, int *pfd, int j)
{
	int		cmd;
	int		bk[2];

	bk[0] = dup(0);
	bk[1] = dup(1);
	dup2(pfd[j - 1], 0);
	close(pfd[j - 1]);
	dup2(pfd[j], 1);
	close(pfd[j]);
	make_a_redirection(data->redir);
	data->path2exec = data->find;
	if ((cmd = is_builtin(data->find)))
	{
		builtin(data, cmd);
	}
	else
	{
		if (!ft_strchr(data->find, '/'))
		{
			data->path2exec = find_in_path(data->find);
			if (!data->path2exec)
			{
				dup2(bk[0], 0);
				close(bk[0]);
				dup2(bk[1], 1);
				close(bk[1]);
				g_bash_errno = E_COMMAND;
				ft_strncpy(g_bash_error, data->find, -1);
				bash_error();
				exit(g_bash_errno);
			}
		}
		execve(data->path2exec, data->args, g_line->envp);
		dup2(bk[0], 0);
		close(bk[0]);
		dup2(bk[1], 1);
		close(bk[1]);
		g_bash_errno = E_ERRNO;
		ft_strncpy(g_bash_error, data->path2exec, -1);
		bash_error();
		exit(errno == 8 ? 126 : g_bash_errno);
	}
	exit(0);
}

/*
** This function gives g_program_return exit status
** of the last executed program so we can use it
** with $?
*/

void	put_exit_status(void)
{
	int		status;

	wait(&status);
	if (WIFEXITED(status))
		g_program_return = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_program_return = WTERMSIG(status);
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
		if (data->find)
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
				put_exit_status();
				if (!CMP(data->find, "cd") && chdir(data->args[1]) < 0)
				{
					g_program_return = 1;
					g_bash_errno = E_ERRNO;
					ft_strncpy(g_bash_error, data->args[1], -1);
					g_bash_commandid = BC_CD; // BASH COMMAND CD
					bash_error();
				}
				else if (!CMP(data->find, "export"))
					bc_export(data);
				else if (!CMP(data->find, "unset"))
					bc_unset(data);
				close(pfd[j - 1]);
				close(pfd[j]);
			}
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
