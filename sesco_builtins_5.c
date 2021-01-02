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

void    print_all_envs(void)
{
	int		env_len;

	env_len = ;
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