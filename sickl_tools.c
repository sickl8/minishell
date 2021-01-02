/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2021/01/01 19:39:10 by isaadi           ###   ########.fr       */
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

size_t	ft_len(void *p, int	size, size_t end)
{
	short	*sp;
	int		*ip;
	long	*lp;
	size_t	ret;

	ret = 0;
	if (size == 1)
		return (ft_strlen((char*)p));
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

int		bc_export_bk(t_cmd *data)
{
	int		argc;
	char	**argv;

	argv = data->args;
	argc = 0;
	return (0);
}