/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/17 16:51:55 by isaadi           ###   ########.fr       */
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

char	**assign_valid_args(char **args, int *valid, int len)
{
	char	**tmp_args;
	int		i;
	int		j;

	if (!(tmp_args = malloc(sizeof(char **) * (len + 2))))
		cleanup(EXIT);
	tmp_args[0] = args[0];
	i = 1;
	j = 1;
	while (i < len + 1)
	{
		if (valid[i - 1] != 1)
		{
			tmp_args[j] = args[i];
			j++;
		}
		i++;
	}
	tmp_args[j] = NULL;
	return (tmp_args);
}

int		bc_export(t_cmd *data)
{
	int			args_len;
	int			*valid_args;
	t_export	lengths;
	t_evar		*tmp;
	char		**tmp_args;

	args_len = count_args(data->args);
	valid_args = check_errors_of_args(data->args, args_len, 1, 0);
	lengths = calc_lengths(valid_args, args_len);
	if (g_cmds_length == 1)
	{
		if (!(tmp = malloc(sizeof(t_evar) * (lengths.new_env_len + 1))))
			cleanup(EXIT);
		tmp_args = assign_valid_args(data->args, valid_args, args_len);
		export_old_vars(tmp_args, lengths, valid_args, tmp);
		free(tmp_args);
		free(valid_args);
		free_envar();
		g_line->env_var = tmp;
	}
	else
		free(valid_args);
	return (0);
}

int		bc_env(void)
{
	int		i;

	i = 0;
	while (g_line->env_var[i].name)
	{
		if (!g_line->env_var[i].name_only)
		{
			OPRINT(g_line->env_var[i].name);
			OPRINTS("=");
			OPRINT(g_line->env_var[i].value);
			OPRINTS("\n");
		}
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
	printf("from echo\n");
	for (int x = 0; data->args[x]; x++)
		printf("arg[%d]=|%s|, ", x, data->args[x]);
	printf("\n");
	if (data->args[i] && !CMP(data->args[i], "-n"))
	{
		pn = 0;
		i++;
	}
	while (data->args[i] && data->args[i + 1])
	{
		if (CMP(data->args[i], "-n"))
		{
			OPRINT(data->args[i]);
			OPRINTS(" ");
		}
		i++;
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

	(void)data;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (-1);
	OPRINT(cwd);
	OPRINTS("\n");
	free(cwd);
	return (0);
}
