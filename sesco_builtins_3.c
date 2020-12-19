/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/19 09:18:50 by aamzouar         ###   ########.fr       */
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

void		duplicated_variable(char *arg_name, int len)
{
	int	i;
	char	tmp[len + 1];

	i = 0;
	// if it's name only without equal don't consider it so i won't add it to env
	ft_strncpy(tmp, arg_name, len);
	tmp[len] = '\0';
	if (arg_name[len - 1] != '\0' && g_dup == 0)
	{
		while (g_line->env_var[i].name)
		{
			if (!CMP(tmp, g_line->env_var[i].name))
			{
				g_dup = i + 1;
				break ;
			}
			i++;
		}
	}
}

int		*check_errors_of_args(char **args, int len, int i, int j)
{
	int		*valid_args;
	char	*name;

	if (!(valid_args = ft_calloc(len, sizeof(int))))
		cleanup(EXIT);
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
		}
		duplicated_variable(args[i], j);
		// if it doesn't have equal on it consider it as invalid
		if (!check_var_name(args[i], j) || args[i][j] == '\0')
			valid_args[i - 1] = 1;
		i++;
	}
	return (valid_args);
}

t_evar	ft_realloc(char *name, char *value)
{
	t_evar	tmp;
	int		name_len;
	int		value_len;

	if (name)
	{
		name_len = ft_strlen(name);
		value_len = ft_strlen(value);
		if (!(MALLOC(tmp.name, name_len + 1)))
			cleanup(EXIT);
		ft_strcpy(tmp.name, name);
		if (!(MALLOC(tmp.value, value_len + 1)))
			cleanup(EXIT);
		ft_strcpy(tmp.value, value);
		tmp.name_len = name_len;
		tmp.value_len = value_len;
	}
	else
	{
		tmp.name = NULL;
		(tmp.value = ft_strdup("")) ? 0 : cleanup(EXIT);
		tmp.name_len = -1;
		tmp.value_len = -1;
	}
	return (tmp);
}

int		bc_cd(t_cmd *data)
{
	if (data->args[1])
	{
		if (chdir(data->args[1]) < 0)
		{
			g_program_return = 1;
			g_bash_errno = E_ERRNO;
			ft_strncpy(g_bash_error, data->args[1], -1);
			g_bash_commandid = BC_CD;
			bash_error();
			return (1);
		}
	}
	else
		return (go_to_home());
	return (0);
}