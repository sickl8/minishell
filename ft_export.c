/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 10:20:44 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/13 16:38:14 by isaadi           ###   ########.fr       */
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
#include "extern.h"
#include "errors.h"

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
			start++;;
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