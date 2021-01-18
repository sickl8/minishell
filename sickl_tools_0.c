/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/18 11:49:29 by isaadi           ###   ########.fr       */
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

int		change_dir(char *s, char *cwd)
{
	t_cmd	d;
	char	op[sizeof("OLDPWD=") + ft_strlen(cwd ? cwd : "")];
	char	*p;
	int		ret;

	s = !CMP(s, "") ? "." : s;
	if (!cwd)
		cleanup(EXIT);
	ret = chdir(s);
	if (!(s = getcwd(NULL, 0)))
		eerf(cwd) && cleanup(EXIT);
	if (!(p = ft_strjoin("PWD=", s)))
		eerf(cwd) && eerf(s) && cleanup(EXIT);
	ft_strcpy(op, "OLDPWD=");
	ft_strcpy(op + sizeof("OLDPWD=") - 1, find_env("PWD").value);
	d.args = (char*[]){ (char*)"export", p, ret < 0 ? NULL : op, NULL};
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

size_t	ft_struct_len(void *p, void *member, size_t end, int size[2])
{
	int		offset;
	size_t	ret;
	int		sz;
	int		szm;

	sz = size[0];
	szm = size[1];
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

void	invalid_arg(char **av, int i, int *rt)
{
	char	*eq;

	eq = ft_strchr(av[i], '=');
	if (eq)
		*eq = '\0';
	if (!env_var_comp(av[i]))
	{
		if (eq)
			*eq = '=';
		g_bash_errno = E_BUILTIN;
		g_builtin_errno = EB_UNSET_EXPORT_NVI;
		g_bash_commandid = BC_EXPORT;
		STRCPY(g_bash_error, av[i] ? av[i] : "");
		g_program_return = 1;
		*rt = 1;
		bash_error();
	}
	if (eq)
		*eq = '=';
}

int		bc_export_bk(t_cmd *data)
{
	int		argc;
	char	**argv;
	t_evar	*tmp;
	int		ret;

	ret = 0;
	argv = &(data->args[1]);
	argc = ft_len(argv, sizeof(*argv), (size_t)NULL);
	argv = sanitize_av(argc, argv, &ret);
	argc = ft_len(argv, sizeof(*argv), (size_t)NULL);
	if (g_cmds_length == 1)
	{
		if (!(tmp = malloc(sizeof(t_evar) * (argc +
		ft_struct_len(g_line->env_var, &g_line->env_var->name, 0, (int[]){
		sizeof(*(g_line->env_var)), sizeof(g_line->env_var->name)}) + 1))))
			eerf(argv) && cleanup(EXIT);
		assign_old_args(assign_valid_args_bk(argc, argv, tmp), tmp);
		free_envar();
		g_line->env_var = tmp;
	}
	free(argv);
	return (0);
}
