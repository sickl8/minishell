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

char	*combine_name_with_value(int j)
{
	char	*env_var;
	char	*tmp;

	env_var = ft_strjoin(g_line->env_var[j].name, "=");
	tmp = env_var;
	env_var = ft_strjoin(env_var, g_line->env_var[j].value);
	free(tmp);
	return (env_var);
}

char	**env_var_copy(char *path2exec)
{
	char	**the_copy;
	int		len;
	int		i;
	int		j;

	len = 0;
	while (g_line->env_var[len].name)
		len++;
	if (!(the_copy = malloc(sizeof(char *) * (len + 2))))
		cleanup(EXIT);
	i = 0;
	j = 0;
	while (g_line->env_var[j].name)
	{
		if (g_line->env_var[j].value && CMP(g_line->env_var[j].name, "_"))
			the_copy[i++] = combine_name_with_value(j);
		j++;
	}
	the_copy[i++] = ft_strjoin("_=", path2exec);
	the_copy[i] = NULL;
	return (the_copy);
}

void	malloc_pid_buffer(t_cmd *data)
{
	int		len;

	len = 0;
	g_pid_group = NULL;
	while (data)
	{
		data = data->next;
		len++;
	}
	if (!(g_pid_group = malloc(sizeof(pid_t) * (len + 1))))
		cleanup(EXIT);
	g_pid_group[len] = -2;
}
