/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:52:43 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/06 18:00:08 by isaadi           ###   ########.fr       */
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

int		count_args(char **args)
{
	int		i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i - 1);
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
		if (!(MALLOC(&ret, len + 2)))
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
** This function counts how many cmds
** are exist and also allocates enough
** memory to the holding array
*/

int		*count_cmds(t_cmd *data)
{
	int		*pipes_fd;

	while (data)
	{
		g_cmds_length++;
		data = data->next;
	}
	if (!(MALLOC(&pipes_fd, g_cmds_length * 8)))
		cleanup(EXIT);
	return (pipes_fd);
}
