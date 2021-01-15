/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 10:20:44 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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

void	set_zero(t_evar *tmp, int end)
{
	int		i;

	i = 0;
	while (i < end)
	{
		tmp[i].name_only = 0;
		i++;
	}
}

void	set_name_only(int start, int *valid, int len, t_evar *tmp)
{
	int		i;

	if (valid)
	{
		i = 0;
		set_zero(tmp, start);
		while (start < len)
		{
			if (valid[i] == 2)
				tmp[start].name_only = 1;
			i++;
			start++;
		}
	}
	else
	{
		while (g_line->env_var[start].name)
		{
			g_line->env_var[start].name_only = 0;
			start++;
		}
	}
}

void	export_new_vars(char **args, int i, t_evar *tmp, int j)
{
	t_evar	new;
	int		k;
	int		old;

	old = i;
	while (args[j])
	{
		k = i;
		new = name_or_value(args[j]);
		while (j != 1 && k--)
		{
			if (ft_strchr(args[j], '=') && !CMP(new.name, tmp[k].name))
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

void	export_old_vars(char **args, t_export len, int *valid, t_evar *tmp)
{
	int		j;
	t_evar	new;
	int		k;
	int		i;

	i = 0;
	j = 0;
	k = 1;
	while (i < len.env_len && g_line->env_var[j].name)
	{
		new = name_or_value(args[k]);
		if (!args[k] || !ft_strchr(args[k], '=') ||
			CMP(new.name, g_line->env_var[j].name))
		{
			tmp[i] =
				ft_realloc(g_line->env_var[j].name, g_line->env_var[j].value);
			i++;
		}
		else
			k++;
		free(new.name);
		j++;
	}
	export_new_vars(args, i, tmp, 1);
	set_name_only(len.env_len, valid, len.new_env_len, tmp);
}
