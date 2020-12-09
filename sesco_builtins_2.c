/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/09 17:14:09 by aamzouar         ###   ########.fr       */
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

int		bc_export_continue(t_cmd *data, int len, t_evar *tmp, char **var_val)
{
	int		i;

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
	if (!(MALLOC(tmp[i].name, (ft_strlen(var_val[0]) + 1))))
		cleanup(EXIT);
	ft_strcpy(tmp[i].name, var_val[0]);
	if (!(MALLOC(tmp[i].value, (ft_strlen(var_val[1]) + 1))))
		cleanup(EXIT);
	ft_strcpy(tmp[i].value, var_val[1]);
	tmp[i].name_len = ft_strlen(var_val[0]);
	tmp[i].value_len = ft_strlen(var_val[1]);
	return (++i);
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
	var_val = ft_split(data->args[1], '=');
	i = bc_export_continue(data, len, tmp, var_val);
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
