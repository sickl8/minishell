/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/10 19:20:30 by isaadi           ###   ########.fr       */
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

int		eerf(void *p)
{
	free(p);
	return (1);
}

int		compare_evars(char *s1, char *s2)
{
	t_evar	var1;
	t_evar	var2;
	int		ret;

	ret = 0;
	var1 = get_evar(s1);
	var2 = get_evar(s2);
	if (env_var_comp(var1.name) && !CMP(var1.name, var2.name) &&
	(!var1.value || var2.value))
		ret = 1;
	s1 = reset_evar(&var1);
	s2 = reset_evar(&var2);
	return (ret);
}


char	**sanitize_av(int ac, char **av)
{
	char	**ret;
	int		cnt;
	int		i;

	cnt = 0;
	i = -1;
	while (av[++i])
		cnt += valid_arg(ac, av, i);
	if (!(ret = malloc((cnt + 1) * sizeof(*ret))))
		cleanup(EXIT);
	ret[cnt] = NULL;
	i = -1;
	cnt = 0;
	while (av[++i])
		if (valid_arg(ac, av, i))
			ret[cnt++] = av[i];
	return (ret);
}

int		valid_arg(int ac, char **av, int index)
{
	int		i;

	i = index;
	while (++i < ac)
	{
		if (compare_evars(av[index], av[i]))
			return (0);
	}
	return (1);
}

int		assign_valid_args_bk(int ac, char **av, t_evar *tmp)
{
	int		i;
	int		j;
	t_evar	var;

	i = -1;
	j = 0;
	while (++i < ac)
	{
		if (valid_arg(ac, av, i))
		{
			var = get_evar(av[i]);
			tmp[j].name = NULL;
			tmp[j].value = NULL;
			tmp[j].name = ft_strdup(var.name);
			if (!tmp[j].name ||
			(var.value && !(tmp[j].value = ft_strdup(var.value))))
				free_tmp_ava(tmp) && cleanup(EXIT);
			tmp[j].name_len = var.name_len;
			tmp[j].value_len = var.value_len;
			av[i] = reset_evar(&var);
			j++;
		}
	}
	return (j);
}