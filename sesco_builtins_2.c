/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 17:08:48 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/06 11:38:46 by aamzouar         ###   ########.fr       */
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

void	export_new_vars(char **args, int i, t_evar *tmp)
{
	int		j;
	t_evar	new;
	int		k;
	int		old;

	j = 1;
	old = i;
	while (args[j])
	{
		k = i;
		new = name_or_value(args[j]);
		while (j != 1 && k--)
		{
			if (!CMP(new.name, tmp[k].name))
			{
				free(tmp[k].name);
				free(tmp[k].value);
				tmp[k] = ft_realloc(new.name, new.value);
				break ;
			}
		}
		if (k == -1 || j == 1)
			tmp[i++] = ft_realloc(new.name, new.value);
		j++;
		free(new.name);
	}
	tmp[i] = ft_realloc(NULL, NULL);
}

void	export_old_vars(char **args, t_export len, int i, t_evar *tmp)
{
	int		j;
	t_evar	new;
	int		k;

	j = 0;
	k = 1;
	while (i < len.env_len && g_line->env_var[j].name)
	{
		new = name_or_value(args[k]);
		if (!args[k] || CMP(new.name, g_line->env_var[j].name)) 
		{
			tmp[i] = ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			i++;
		}
		else
			k++;
		free(new.name);
		j++;
	}
	export_new_vars(args, i, tmp);
}

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
		if (valid[i - 1] == 0)
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
		export_old_vars(tmp_args, lengths, 0, tmp);
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
