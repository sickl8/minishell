/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:07:13 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/14 18:43:21 by isaadi           ###   ########.fr       */
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

int		bc_unset_continue(t_cmd *data, t_evar *tmp, int len)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (i < len)
	{
		if (CMP(g_line->env_var[i].name, data->args[1]))
		{
			if (!(MALLOC(tmp[j].name,
							(ft_strlen(g_line->env_var[i].name) + 1))))
				cleanup(EXIT);
			ft_strcpy(tmp[j].name, g_line->env_var[i].name);
			if (!(MALLOC(tmp[j].value,
							(ft_strlen(g_line->env_var[i].value) + 1))))
				cleanup(EXIT);
			ft_strcpy(tmp[j].value, g_line->env_var[i].value);
			tmp[j].name_len = g_line->env_var[i].name_len;
			tmp[j].value_len = g_line->env_var[i].value_len;
			j++;
		}
		i++;
	}
	return (j);
}

int		bc_unset(t_cmd *data)
{
	int		len;
	t_evar	*tmp;
	int		j;

	len = 0;
	while (g_line->env_var[len].name != NULL)
		len++;
	if (!(MALLOC(tmp, len)))
		cleanup(EXIT);
	j = bc_unset_continue(data, tmp, len);
	tmp[j].name = NULL;
	tmp[j].value = NULL;
	tmp[j].name_len = -1;
	tmp[j].value_len = -1;
	free(g_line->env_var);
	g_line->env_var = tmp;
	return (0);
}

int		builtin(t_cmd *data, int cmd)
{
	int		ret;

	if (cmd == BC_CD)
		ret = 0;
	else if (cmd == BC_ECHO)
		ret = bc_echo(data);
	else if (cmd == BC_ENV)
		ret = bc_env();
	else if (cmd == BC_EXIT)
		ret = 0;
	else if (cmd == BC_EXPORT)
		ret = 0;
	else if (cmd == BC_PWD)
		ret = bc_pwd(data);
	else if (cmd == BC_UNSET)
		ret = 0;
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
