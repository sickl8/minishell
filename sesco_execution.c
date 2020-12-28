/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:01:33 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/18 20:08:20 by aamzouar         ###   ########.fr       */
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
** Find the command in $PATH
*/

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
			if (!CMP(tofind, file->d_name))
			{
				paths[i] = fix_path(paths, i);
				if (!(ret = ft_strjoin(paths[i], file->d_name)))
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
	if (!(paths = ft_split(path.value, ':'))) // paths is automatically freed on any NULL malloc return
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

/*
** here we check if it's a path or just a command
** path -> [/]
** command -> [no slash]
** also it redirects pipes
*/

void	execute_cmd_continue(t_cmd *data, int bk[2])
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
		builtin(data, cmd);
	else
		execute_cmd_continue(data, bk);
	exit(0);
}
