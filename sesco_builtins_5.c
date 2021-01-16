/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 18:51:48 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/16 16:39:29 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_evar	*create_env_list(int env_len)
{
	t_evar		*env_copy;
	int			i;

	if (!env_len)
		return (g_line->env_var);
	if (!(env_copy = malloc(sizeof(t_evar) * env_len)))
		cleanup(EXIT);
	i = 0;
	while (i < env_len)
	{
		env_copy[i] = g_line->env_var[i];
		i++;
	}
	return (env_copy);
}

void	sort_env_list(t_evar *env_copy)
{
	int		i;
	int		j;
	t_evar	tmp_env;

	i = 0;
	while (g_line->env_var[i].name)
	{
		j = i + 1;
		while (g_line->env_var[j].name)
		{
			if (CMP(env_copy[i].name, env_copy[j].name) > 0)
			{
				tmp_env = env_copy[i];
				env_copy[i] = env_copy[j];
				env_copy[j] = tmp_env;
			}
			j++;
		}
		i++;
	}
}

void	print_env_list(t_evar *env_copy, int env_len)
{
	int		i;
	int		j;

	i = -1;
	while (++i < env_len)
	{
		OPRINT("declare -x ");
		OPRINT(env_copy[i].name);
		if (!env_copy[i].name_only)
		{
			j = -1 + 0 * OPRINTS("=\"");
			while (env_copy[i].value[++j])
			{
				if (env_copy[i].value[j] == '$' || env_copy[i].value[j] == '\\'
				|| env_copy[i].value[j] == '"')
					OPRINTC('\\');
				OPRINTC(env_copy[i].value[j]);
			}
			OPRINT("\"");
			OPRINTS("\n");
		}
		else
			OPRINTS("\n");
	}
}

int		print_all_envs(t_cmd *data)
{
	t_evar		*env_copy;
	int			env_len;

	if (data->args[1])
		return (0);
	env_len = 0;
	while (g_line->env_var[env_len].name)
		env_len++;
	env_copy = create_env_list(env_len);
	if (env_copy[0].name)
	{
		sort_env_list(env_copy);
		print_env_list(env_copy, env_len);
	}
	free(env_copy);
	return (0);
}

void	*failing_error(t_cmd *data)
{
	(void)data;
	EPRINTS("minishell: ");
	EPRINT(strerror(errno));
	EPRINTS("\n");
	return (NULL);
}
