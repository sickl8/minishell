/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sesco_builtins_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 18:22:33 by aamzouar          #+#    #+#             */
/*   Updated: 2021/01/19 14:48:05 by isaadi           ###   ########.fr       */
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

int			go_to(t_cmd *data)
{
	t_evar	var;

	var = data->args[1] && CMP(data->args[1], "--") ? find_env("OLDPWD") :
	find_env("HOME");
	if (!var.name)
		return (1);
	!(var.value = ft_strdup(var.value)) ? cleanup(EXIT) : 0;
	if (change_dir(var.value, getcwd(NULL, 0)) < 0)
		return (1);
	free(var.value);
	return (0);
}

t_evar		name_or_value(char *arg)
{
	t_evar	ret;

	ret.name = NULL;
	ret.value = NULL;
	if (arg)
	{
		if (!(ret.name = ft_strdup(arg)))
			cleanup(EXIT);
		ret.value = ft_strchr(ret.name, '=');
		if (ret.value)
		{
			ret.value[0] = '\0';
			ret.value = ret.value + 1;
		}
		else
			ret.value = ret.name + ft_strlen(ret.name);
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
	while (g_line->env_var[res.env_len].name)
		res.env_len++;
	res.new_env_len = 0;
	i = 0;
	while (i < len)
		if (valid[i++] != 1)
			res.new_env_len++;
	res.new_env_len += res.env_len;
	return (res);
}

int			check_if_num(char *s)
{
	int		i;
	int		sign;

	i = 0;
	while (s[i] && (s[i] == 32 || s[i] == 9 || s[i] == 12 || s[i] == 13))
		i++;
	sign = 1;
	if (!ISNUM(s[i]) && s[i] != '+' && s[i] != '-')
		return (1);
	else if (s[i] == '-')
		sign = -1;
	if (!ISNUM(s[i]))
		i++;
	if (!ISNUM(s[i]))
		return (1);
	while (s[i] && s[i] == '0')
		i++;
	assign(&s, (t_ull)(s + i), sizeof(s)) && assign(&i, 0, sizeof(i));
	while (s[i] && ISNUM(s[i]))
		i++;
	i--;
	while (s[++i])
		if (!(s[i] && (s[i] == 32 || s[i] == '\t')))
			return (1);
	return (continue_check_num(s, sign));
}

int			bc_exit(char **argv, t_cmd *data)
{
	int		argc;
	int		exit_status;

	argc = (int)ft_len(argv, sizeof(*argv), 0);
	assign(&g_bash_commandid, BC_EXIT, 4) && assign(&g_bash_errno, E_BUILTN, 4);
	if (argc > 1 && check_if_num(argv[1]))
	{
		!data->next ? g_program_return = 255 : 0;
		g_builtin_errno = EB_EXIT_NAR;
		ft_strcpy(g_bash_error, argv[1]);
		bash_error();
		g_cmds_length == 1 ? cleanup(255) : 0;
	}
	else if (argc > 2)
	{
		!data->next ? g_program_return = 1 : 0;
		g_builtin_errno = EB_CD_EXIT_TMA;
		bash_error();
		return (1);
	}
	exit_status = argc == 1 ? g_program_return : ft_atoi(argv[1]);
	g_cmds_length == 1 ? cleanup(RETURN) : 0;
	g_cmds_length == 1 ? exit(exit_status % 256) : 0;
	return (0);
}
