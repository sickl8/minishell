/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickl8 <sickl8@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:07:13 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/31 18:57:09 by sickl8           ###   ########.fr       */
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

void	unset_var(char **args, t_export len, t_evar *tmp)
{
	int		j;
	int		i;
	int     k;

	j = 0;
	i = 0;
	k = 1;
	while (i < len.env_len && g_line->env_var[j].name)
	{
		if (args[k] && CMP(args[k], g_line->env_var[j].name)) 
		{
			tmp[i] = ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			i++;
		}
		else
			k++;
		j++;
	}
	tmp[i] = ft_realloc(NULL, NULL);
}

int		bc_unset(t_cmd *data)
{
	int			args_len;
	int			*valid_args;
	t_export	lengths;
	t_evar		*tmp;

	g_dup = 0;
	args_len = count_args(data->args);
	valid_args = check_errors_of_args(data->args, args_len, 1, 0);
	lengths = calc_lengths(valid_args, args_len);
	if (g_cmds_length == 1)
	{
		if (!(tmp = malloc(sizeof(t_evar) * (lengths.env_len + 1))))
			cleanup(EXIT);
		unset_var(data->args, lengths, tmp);
		free(valid_args);
		free_envar();
		g_line->env_var = tmp;
	}
	else
		free(valid_args);
	return (0);
}

int		builtin(t_cmd *data, int cmd)
{
	int		ret;

	if (cmd == BC_CD)
		ret = bc_cd_fork(data);
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