/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickl8 <sickl8@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 18:51:48 by sickl8            #+#    #+#             */
/*   Updated: 2020/12/31 18:54:36 by sickl8           ###   ########.fr       */
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
#include "proto.h"
#include "typedef.h"
#include "def.h"
#include "global.h"
#include "errors.h"

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

	i = 0;
	while (i < env_len)
	{
		OPRINT("declare -x ");
		OPRINT(env_copy[i].name);
		OPRINTS("=");
		OPRINT("\"");
		OPRINT(env_copy[i].value);
		OPRINT("\"");
		OPRINTS("\n");
		i++;
	}
}

void    print_all_envs(void)
{
	t_evar		*env_copy;
	int			env_len;

	env_len = 0;
	while (g_line->env_var[env_len].name)
		env_len++;
	// if there is no env variable
	env_copy = create_env_list(env_len);
	if (env_copy[0].name)
	{
		sort_env_list(env_copy);
		print_env_list(env_copy, env_len);
	}
}

/*
** showing an error if FORK or PIPE failed !
*/

void    *failing_error(t_cmd *data)
{
	EPRINTS("minishell: ");
	EPRINT(strerror(errno));
	EPRINTS("\n");
	return (NULL);
}

int		go_to_fork(t_cmd *data)
{
	t_evar	var;

	var = data->args[1] ? find_env("OLDPWD") : find_env("HOME");
	if (!var.name)
	{
		A(g_program_return, 1) && A(g_bash_errno, E_BUILTIN);
		ft_strncpy(g_bash_error, var.value, -1);
		g_builtin_errno = data->args[1] ? EB_CD_ONT : EB_CD_HNT;
		g_bash_commandid = BC_CD;
		bash_error();
		return (1);
	}
	var.value_len = !(var.value = ft_strdup(var.value)) ?
	cleanup(EXIT) : var.value_len;
	if (change_dir(var.value, getcwd(NULL, 0)) < 0)
	{
		A(g_program_return, 1) && A(g_bash_errno, E_ERRNO);
		ft_strncpy(g_bash_error, var.value, -1);
		g_bash_commandid = BC_CD;
		bash_error();
		free(var.value);
		return (1);
	}
	free(var.value);
	return (0);
}

int		bc_cd_fork(t_cmd *data)
{
	if (data->args[1] && CMP(data->args[1], "-"))
	{
		if (change_dir(data->args[1], getcwd(NULL, 0)) < 0)
		{
			g_bash_errno = E_ERRNO;
			ft_strncpy(g_bash_error, data->args[1], -1);
			g_bash_commandid = BC_CD;
			bash_error();
			return (1);
		}
	}
	else
		return (go_to_fork(data));
	return (0);
}