/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamzouar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 18:22:33 by aamzouar          #+#    #+#             */
/*   Updated: 2020/12/18 18:48:54 by aamzouar         ###   ########.fr       */
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

char	*name_or_value(int sign, char *arg)
{
	char	*ret;
	int		i;
	int		equal;

	i = 0;
	equal = 0;
	ret = NULL;
	while (arg[equal] != '=')
		equal++;
	if (sign == 0)
	{
		if (!(MALLOC(ret, sizeof(char) * equal + 1)))
			cleanup(EXIT);
		ret = ft_strncpy(ret, arg, equal);
		ret[equal] = '\0';
	}
	else if (sign == 1)
	{
		equal += 1;
		if (!(MALLOC(ret, sizeof(char) * ft_strlen(arg + equal) + 1)))
			cleanup(EXIT);
		ft_strcpy(ret, arg + equal);
	}
	return (ret);
}

t_export	calc_lengths(int *valid, int len)
{
	int			i;
	t_export	res;

	i = 0;
	res.env_len = 0;
	res.new_env_len = 0;
	// count existed env variables
	while (g_line->env_var[res.env_len].name)
		res.env_len++;
	// add the valid args
	res.new_env_len = 0;
	i = 0;
	while (i < len)
		if (valid[i++] == 0)
			res.new_env_len++;
	// remove from the existing ones
	res.env_len -= g_dup ? 1 : 0;
	res.new_env_len += res.env_len;
	return (res);
}

int		check_if_num(char *s)
{
	int		i;

	i = 0;
	while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		i++;
	if (!ISNUM(s[i]) && s[i] != '+' && s[i] != '-')
		return (1);
	if (!ISNUM(s[i]))
		i++;
	if (!ISNUM(s[i]))
		return (1);
	while (s[i] && ISNUM(s[i]))
		i++;
	while (s[i])
	{
		if (!(s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))))
			return (1);
		i++;
	}
	return (0);
}

int		bc_exit(char **argv)
{
	int		argc;
	int		exit_status;

	OPRINTS("exit\n");
	argc = 0;
	while (argv[argc])
		argc++;
	g_bash_commandid = BC_EXIT;
	g_bash_errno = E_BUILTIN;
	if (argc > 1 && check_if_num(argv[1]))
	{
		g_builtin_errno = EB_EXIT_NAR;
		ft_strcpy(g_bash_error, argv[1]);
		bash_error();
		cleanup(2);
	}
	if (argc > 2)
	{
		g_builtin_errno = EB_CD_EXIT_TMA;
		bash_error();
		return (1);
	}
	exit_status = argc == 1 ? 0 : ft_atoi(argv[1]);
	cleanup(RETURN);
	exit(exit_status % 256);
}
