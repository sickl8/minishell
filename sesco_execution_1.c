/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:01:33 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/18 17:32:25 by isaadi           ###   ########.fr       */
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

char	*find_in_single_path(char *tofind, char **paths, int i)
{
	DIR				*dir;
	struct dirent	*file;
	char			*ret;

	ret = NULL;
	dir = opendir(paths[i]);
	if (dir)
	{
		while ((file = readdir(dir)) && ret == NULL)
		{
			if (!CASE_CMP(tofind, file->d_name))
			{
				paths[i] = fix_path(paths, i);
				if (!(ret = ft_strjoin(paths[i], tofind)))
				{
					free_path(paths);
					cleanup(EXIT);
				}
			}
		}
		closedir(dir);
	}
	return (ret);
}

char	*find_in_path(char *tofind)
{
	t_evar			path;
	char			**paths;
	int				i;
	char			*ret;

	path = find_env("PATH");
	if (!(paths = ft_split(path.value, ':')))
		cleanup(EXIT);
	ret = NULL;
	i = 0;
	while (paths[i] && ret == NULL)
	{
		ret = find_in_single_path(tofind, paths, i);
		i++;
	}
	free_path(paths);
	return (ret);
}

void	print_execution_errors(char *cmd, int bk[2])
{
	struct stat f_info;

	if (stat(cmd, &f_info) < 0)
	{
		g_bash_errno = E_ERRNO;
		g_bash_commandid = BC_DEF;
		ft_strncpy(g_bash_error, cmd, -1);
		bash_error();
		exit(127);
	}
	if (S_ISREG(f_info.st_mode))
	{
		if ((f_info.st_mode & S_IRUSR) && (f_info.st_mode & S_IXUSR))
			exit(0);
		errno == 8 && (f_info.st_mode & S_IXUSR) ? errno = 13 : 0;
	}
	S_ISDIR(f_info.st_mode) ? errno = 21 : 0;
	dup2(bk[0], 0);
	close(bk[0]);
	dup2(bk[1], 1);
	close(bk[1]);
	g_bash_errno = E_ERRNO;
	ft_strncpy(g_bash_error, cmd, -1);
	bash_error();
	exit(errno == 8 || errno == 13 || errno == 21 ? 126 : g_bash_errno);
}

void	execute_cmd_continue(t_cmd *data, int bk[2])
{
	data->path2exec = data->find;
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
	execve(data->path2exec, data->args, env_var_copy(data->path2exec));
	print_execution_errors(data->path2exec, bk);
}

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
	if (data->find)
	{
		if ((cmd = is_builtin(data->find)))
			builtin(data, cmd);
		else
			execute_cmd_continue(data, bk);
	}
	exit(0);
}
