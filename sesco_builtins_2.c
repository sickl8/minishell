/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/17 00:20:19 by aamzouar         ###   ########.fr       */
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

int		check_var_name(char *name, int end)
{
	int		i;

	i = 0;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (0);
	i++;
	while (i < end)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int		*check_errors_of_args(char **args, int len)
{
	int		*valid_args;
	char	*name;
	int		j;
	int		i;

	if (!(valid_args = ft_calloc(len, sizeof(int))))
		cleanup(EXIT);
	i = 1;
	while (i < len + 1)
	{
		j = 0;
		while (args[i][j] != '=' && args[i][j] != '\0')
			j++;
		if (!check_var_name(args[i], j))
		{
			g_bash_errno = E_BUILTIN;
			g_builtin_errno = EB_UNSET_EXPORT_NVI;
			g_bash_commandid = BC_EXPORT;
			STRCPY(g_bash_error, args[i] ? args[i] : "");
			g_program_return = 1;
			bash_error();
			valid_args[i - 1] = 1;
		}
		i++;
	}
	return (valid_args);
}

int		bc_export(t_cmd *data)
{
	int		args_len;
	int		*valid_args;

	args_len = count_args(data->args);
	valid_args = check_errors_of_args(data->args, args_len);
	free(valid_args);
	return (0);
}

/*
int		bc_export(t_cmd *data)
{
	t_evar	var;
	char	*tmp;

	var.name = data->args[1];
	var.value = data->args[1];
	tmp = ft_strchr(data->args[1], '=');
	if (!env_var_comp(var.name))
	{
		g_bash_errno = E_BUILTIN;
		g_builtin_errno = EB_UNSET_EXPORT_NVI;
		g_bash_commandid = BC_EXPORT;
		STRCPY(g_bash_error, data->args[1] ? data->args[1] : "");
		bash_error();
		//continue;
	}
	else if (tmp)
	{
		*tmp = '\0';
		var.value = tmp + 1;
		var.name_len = ft_strlen(var.name);
		var.value_len = ft_strlen(var.value);
		//var is ready

	}
	return (0);
}*/

int		bc_env(void)
{
	int		i;

	i = 0;
	while (g_line->env_var[i].name)
	{
		OPRINT(g_line->env_var[i].name);
		OPRINTS("=");
		OPRINT(g_line->env_var[i].value);
		OPRINTS("\n");
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
		OPRINT(data->args[i]);
		i++;
		OPRINTS(" ");
	}
	if (data->args[i])
		OPRINT(data->args[i]);
	if (pn)
		OPRINT("\n");
	return (0);
}

int		bc_pwd(t_cmd *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	OPRINT(cwd);
	OPRINTS("\n");
	free(cwd);
	return (0);
}
