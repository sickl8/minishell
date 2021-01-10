/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickl8 <sickl8@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/10 01:58:03 by sickl8           ###   ########.fr       */
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

int		change_dir(char *s, char *cwd)
{
	t_cmd	d;
	char	op[sizeof "OLDPWD=" + ft_strlen(cwd ? cwd : "")];
	char	*p;
	int		ret;

	if (!cwd)
		cleanup(EXIT);
	ret = chdir(s);
	if (!(s = getcwd(NULL, 0)))
	{
		free(cwd);
		cleanup(EXIT);
	}
	if (!(p = ft_strjoin("PWD=", s)))
	{
		eerf(cwd) && eerf(s);
		cleanup(EXIT);
	}
	ft_strcpy(op, "OLDPWD=");
	ft_strcpy(op + sizeof "OLDPWD=" - 1, cwd);
	d.args = (char*[]){ (char*)"export", op, p, NULL};
	bc_export(&d);
	eerf(s) && eerf(cwd) && eerf(p);
	return (ret);
}

size_t	ft_len(void *p, int size, size_t end)
{
	short	*sp;
	int		*ip;
	long	*lp;
	size_t	ret;

	ret = 0;
	if (size == 1)
		return ((size_t)((void*)ft_strchr(p, (int)end) - p));
	else if (size == 2 && (sp = (short*)p))
	{
		while (sp[ret] != (short)end)
			ret++;
	}
	else if (size == 4 && (ip = (int*)p))
	{
		while (ip[ret] != (int)end)
			ret++;
	}
	else if (size == 8 && (lp = (long*)p))
	{
		while (lp[ret] != (long)end)
			ret++;
	}
	return (ret);
}

size_t	ft_struct_len(void *p, int sz, void *member, int szm, size_t end)
{
	int		offset;
	size_t	ret;

	ret = 0;
	offset = member - p;
	while (1)
	{
		if ((szm == 1 && ((char*)(p + offset))[0] == (char)end) ||
			(szm == 2 && ((short*)(p + offset))[0] == (short)end) ||
			(szm == 4 && ((int*)(p + offset))[0] == (int)end) ||
			(szm == 8 && ((long*)(p + offset))[0] == (long)end))
				break ;
		p += sz;
		ret++;
	}
	return (ret);
}

int		check_valid_args(int ac, char **av)
{
	int		i;
	char	*eq;
	int		ret;

	i = 0;
	ret = 0;
	while (++i < ac)
	{
		eq = ft_strchr(av[i], '=');
		if (eq)
			*eq = '\0';
		if (env_var_comp(av[i]))
		{
			ret++;
			g_bash_errno = E_BUILTIN;
			g_builtin_errno = EB_UNSET_EXPORT_NVI;
			g_bash_commandid = BC_EXPORT;
			STRCPY(g_bash_error, av[i] ? av[i] : "");
			g_program_return = 1;
			bash_error();
		}
		if (eq)
			*eq = '=';
	}
	return (ac - 1 - ret);
}

int		bc_export_bk(t_cmd *data)
{
	int		argc;
	char	**argv;
	t_evar	*tmp;
	int		va;

	argv = data->args;
	argc = ft_len(argv, sizeof(*argv), NULL);
	va = check_valid_args(argc, argv);
	if (g_cmds_length == 1)
	{
		if (!(tmp = malloc(sizeof(t_evar) * (va + ft_struct_len(g_line->env_var, sizeof(g_line->env_var), &g_line->env_var->name, sizeof(g_line->env_var->name), NULL) + 1))))
			cleanup(EXIT);
		// tmp_args = assign_valid_args(data->args, valid_args, args_len);
		// export_old_vars(tmp_args, lengths, valid_args, tmp);
		// free(tmp_args);
		// free(valid_args);
		free_envar();
		g_line->env_var = tmp;
	}
	return (0);
}