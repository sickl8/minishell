/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/17 16:27:07 by isaadi           ###   ########.fr       */
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
	if (!CMP(var1.name, var2.name) && (!var1.value || var2.value))
		ret = 1;
	s1 = reset_evar(&var1);
	s2 = reset_evar(&var2);
	return (ret);
}

char	**sanitize_av(int ac, char **av, int *rt)
{
	char	**ret;
	int		cnt;
	int		i;
	int		tmp;

	cnt = 0;
	g_ez = NULL;
	i = -1;
	while (av[++i])
	{
		!(tmp = valid_arg(ac, av, i)) ? invalid_arg(av, i, rt) : 0;
		// printf("av[i] = %s, tmp = %d\n", av[i], tmp);
		cnt += tmp;
	}
	if (!(ret = malloc((cnt + 1) * sizeof(*ret))))
		cleanup(EXIT);
	ret[cnt] = NULL;
	i = -1;
	cnt = 0;
	while (av[++i])
		if (valid_arg(ac, av, i))
			ret[cnt++] = av[i];
	// for (int x = 0; ret[x]; x++)
	// {
	// 	printf("ret[%d] = |%s|\n", x, ret[x]);
	// }
	return (ret);
}

int		valid_arg(int ac, char **av, int index)
{
	int		i;
	t_evar	var;
	t_evar	*evarp;

	var = get_evar(av[index]);
	if (!env_var_comp(var.name))
		return (0 * !(av[index] = reset_evar(&var)));
	i = var.value ? index : -1;
	evarp = find_env_p(var.name);
	if (evarp)
	{
		if (var.value)
		{
			free_and_set_to_null(&(evarp->value));
			evarp->name_only = 0;
			!(evarp->value = DUP(var.value)) ? eerf(g_ez) && cleanup(EXIT) : 0;
		}
		return (0 * !(av[index] = reset_evar(&var)));
	}
	av[index] = reset_evar(&var);
	while (++i < ac)
		if (index != i ? compare_evars(av[index], av[i]) : 0)
			return (0);
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
		g_ez = tmp;
		if (valid_arg(ac, av, i))
		{
			var = get_evar(av[i]);
			tmp[j].name = NULL;
			tmp[j].value = NULL;
			tmp[j].name_only = var.name_only;
			tmp[j].name = ft_strdup(var.name);
			if (!tmp[j].name ||
			(var.value && !(tmp[j].value = ft_strdup(var.value))))
				free_tmp_ava(tmp) && cleanup(EXIT);
			tmp[j].name_len = var.name_len;
			tmp[j++].value_len = var.value_len;
			av[i] = reset_evar(&var);
		}
	}
	return (j);
}
