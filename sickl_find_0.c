/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_find_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 14:33:06 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:33:22 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_extern.h"
#include "header_errors.h"

#include "header_get_next_line.h"

#include <stdio.h>

t_evar	find_env_wv(char *s)
{
	size_t	j;
	t_evar	ret;
	char	*tmp;

	j = 0;
	if (!CMP(s, "?"))
	{
		ret.name = s;
		ret.name_len = 1;
		if (!(tmp = ft_itoa(g_program_return)))
			cleanup(EXIT);
		ft_memset(g_s_program_return, 0, 100);
		ft_strncpy(g_s_program_return, tmp, -1);
		free(tmp);
		ret.value = g_s_program_return;
		ret.value_len = ft_strlen(ret.value);
		return (ret);
	}
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s) && g_line->env_var[j].value)
			break ;
		j++;
	}
	return (g_line->env_var[j]);
}

t_evar	*find_env_p(char *s)
{
	size_t	j;

	j = 0;
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s))
			return (&g_line->env_var[j]);
		j++;
	}
	return (NULL);
}

t_evar	find_env(char *s)
{
	size_t	j;
	t_evar	ret;
	char	*tmp;

	j = 0;
	if (!CMP(s, "?"))
	{
		ret.name = s;
		ret.name_len = 1;
		if (!(tmp = ft_itoa(g_program_return)))
			cleanup(EXIT);
		ft_memset(g_s_program_return, 0, 100);
		ft_strncpy(g_s_program_return, tmp, -1);
		free(tmp);
		ret.value = g_s_program_return;
		ret.value_len = ft_strlen(ret.value);
		return (ret);
	}
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s))
			break ;
		j++;
	}
	return (g_line->env_var[j]);
}

t_evar	find_env_in_line(size_t *ref)
{
	t_evar	ret;
	size_t	i;
	size_t	j;
	char	env_space[ENV_NAME_LEN_MAX];

	(*ref)++;
	i = *ref;
	j = 0;
	while (g_line->rd.buf[i] && g_line->rd.msk[i] == ENVVAR &&
	i - *ref < ENV_NAME_LEN_MAX)
		env_space[j++] = g_line->rd.buf[i++];
	env_space[j] = '\0';
	ret = find_env(env_space);
	if (!ret.name)
	{
		ret.name_len = ft_strlen(env_space);
		ret.value_len = 0;
	}
	*ref = --i;
	return (ret);
}
