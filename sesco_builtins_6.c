/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 18:51:48 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/17 16:42:55 by isaadi           ###   ########.fr       */
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

int		go_to_fork(t_cmd *data)
{
	t_evar	var;

	var = data->args[1] && CMP(data->args[1], "--") ? find_env("OLDPWD") :
	find_env("HOME");
	if (!var.name)
	{
		assign(&g_program_return, 1, 4) && assign(&g_bash_errno, E_BUILTIN, 4);
		ft_strncpy(g_bash_error, var.value, -1);
		g_builtin_errno = data->args[1] ? EB_CD_ONT : EB_CD_HNT;
		g_bash_commandid = BC_CD;
		bash_error();
		return (1);
	}
	!(var.value = ft_strdup(var.value)) ? cleanup(EXIT) : 0;
	if (chdir(var.value) < 0)
	{
		assign(&g_program_return, 1, 4) && assign(&g_bash_errno, E_BUILTIN, 4);
		ft_strncpy(g_bash_error, var.value, -1);
		g_bash_commandid = BC_CD;
		g_builtin_errno = -1;
		bash_error();
		return (eerf(var.value) * 0 + 1);
	}
	(!CMP(var.name, "OLDPWD") ? OPRINT(var.value) && OPRINT("\n") : 1);
	return (0 * eerf(var.value));
}

int		bc_cd_fork(t_cmd *data)
{
	if (data->args[1] && CMP(data->args[1], "-") && CMP(data->args[1], "--"))
	{
		if (chdir(data->args[1]) < 0)
		{
			g_bash_errno = E_BUILTIN;
			g_bash_commandid = BC_CD;
			g_builtin_errno = -1;
			ft_strncpy(g_bash_error, data->args[1], -1);
			bash_error();
			return (1);
		}
	}
	else
		return (go_to_fork(data));
	return (0);
}
