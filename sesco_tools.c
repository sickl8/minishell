/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:52:43 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/18 17:48:57 by isaadi           ###   ########.fr       */
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

int		count_args(char **args)
{
	int		i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i - 1);
}

char	*ft_strcpy(char *dst, char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

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

void	free_path(char **paths)
{
	int		i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

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
