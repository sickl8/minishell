/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:07:13 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/16 14:47:21 by isaadi           ###   ########.fr       */
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

int		*find_index_of_unset_var(char **args, int len, int i)
{
	int		*indexes;
	int		j;

	while (args[len + 1])
		len++;
	if (!(indexes = ft_calloc(len + 1, sizeof(int))))
		cleanup(EXIT);
	while (i < len)
	{
		j = 0;
		while (g_line->env_var[j].name)
		{
			if (!CMP(args[i + 1], g_line->env_var[j].name))
			{
				indexes[i] = j;
				break ;
			}
			j++;
		}
		indexes[i] == 0 && j != 0 ? indexes[i] = -1 : 0;
		i++;
	}
	return (indexes);
}

void	unset_var(char **args, t_evar *tmp)
{
	int		j;
	int		i;
	int		k;
	int		*indexes;

	j = 0;
	i = 0;
	k = 0;
	indexes = find_index_of_unset_var(args, 0, 0);
	while (g_line->env_var[j].name)
	{
		if (indexes[k] != j)
		{
			tmp[i] =
				ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			tmp[i].name_only = g_line->env_var[j].name_only;
			i++;
		}
		else
			k++;
		j++;
	}
	free(indexes);
	tmp[i] = ft_realloc(NULL, NULL);
}

int		bc_unset(t_cmd *data)
{
	int			args_len;
	int			*valid_args;
	t_export	lengths;
	t_evar		*tmp;
	char		**tmp_args;

	args_len = count_args(data->args);
	valid_args = check_errors_of_unset(data->args, args_len, 1, 0);
	lengths = calc_lengths(valid_args, args_len);
	if (g_cmds_length == 1 && data->args[1])
	{
		if (!(tmp = malloc(sizeof(t_evar) * (lengths.env_len + 1))))
			cleanup(EXIT);
		tmp_args = assign_valid_args(data->args, valid_args, args_len);
		unset_var(tmp_args, tmp);
		free(tmp_args);
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

	ret = 0;
	if (cmd == BC_CD)
		ret = bc_cd_fork(data);
	else if (cmd == BC_ECHO)
		ret = bc_echo(data);
	else if (cmd == BC_ENV)
		ret = bc_env();
	else if (cmd == BC_EXIT)
		ret = 0;
	else if (cmd == BC_EXPORT)
		ret = print_all_envs(data);
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
