/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/18 18:23:04 by aamzouar         ###   ########.fr       */
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

void	export_new_vars(char **args, t_export len, int i, t_evar *tmp)
{
	int		j;
	char	*name;
	char	*value;

	j = 0;
	while (i < len.env_len)
	{
		if (g_dup == 0 || j != (g_dup - 1))
		{
			tmp[i] = ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			i++;
		}
		j++;
	}
	j = 1;
	while (i < len.new_env_len)
	{
		name = name_or_value(0, args[j]);
		value = name_or_value(1, args[j++]);
		tmp[i++] = ft_realloc(name, value);
		free(name);
		free(value);
	}
	tmp[i] = ft_realloc(NULL, NULL);
}

int		bc_export(t_cmd *data)
{
	int			args_len;
	int			*valid_args;
	t_export	lengths;
	t_evar		*tmp;

	g_dup = 0;
	args_len = count_args(data->args);
	valid_args = check_errors_of_args(data->args, args_len, 1, 0);
	lengths = calc_lengths(valid_args, args_len);
	if (!(tmp = malloc(sizeof(t_evar) * (lengths.new_env_len + 1))))
		cleanup(EXIT);
	export_new_vars(data->args, lengths, 0, tmp);
	free(valid_args);
	free_envar();
	g_line->env_var = tmp;
	return (0);
}

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
