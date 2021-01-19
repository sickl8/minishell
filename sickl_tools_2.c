/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/19 18:25:55 by isaadi           ###   ########.fr       */
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

int		find_tmp_env(char *name, int x)
{
	int		i;

	i = -1;
	while (++i < x)
	{
		if (!CMP(name, g_line->env_var[i].name))
			return (1);
	}
	return (0);
}

int		free_tmp_ava(t_evar *tmp)
{
	int		i;

	i = -1;
	while (tmp[++i].name)
		eerf(tmp[i].name) && eerf(tmp[i].value);
	free(tmp);
	return (1);
}

char	*reset_evar(t_evar *var)
{
	if (var->value && var->name)
		var->name[var->name_len] = '=';
	return (var->name);
}

t_evar	get_evar(char *s)
{
	t_evar	ret;
	char	*eq;

	ret.name = s;
	ret.value = NULL;
	ret.value_len = 0;
	eq = ft_strchr(s, '=');
	ret.name_only = 1;
	if (eq)
	{
		*eq = '\0';
		ret.name_only = 0;
		ret.value = eq + 1;
		ret.value_len = ft_strlen(ret.value);
	}
	ret.name_len = ft_strlen(ret.name);
	return (ret);
}

void	assign_old_args(int x, t_evar *tmp)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (g_line->env_var[++i].name)
	{
		tmp[x + j + i] = g_line->env_var[i];
		if ((tmp[x + j + i].name &&
		!(tmp[x + j + i].name = ft_strdup(tmp[x + j + i].name))) ||
		(tmp[x + j + i].value &&
		!(tmp[x + j + i].value = ft_strdup(tmp[x + j + i].value))))
			free_tmp_ava(tmp) && cleanup(EXIT);
	}
	tmp[x + j + i] = g_line->env_var[i];
	if (!(tmp[x + j + i].value = ft_strdup("")))
		free_tmp_ava(tmp) && cleanup(EXIT);
}
