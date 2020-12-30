/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickl8 <sickl8@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:23:01 by sickl8            #+#    #+#             */
/*   Updated: 2020/12/30 18:18:07 by sickl8           ###   ########.fr       */
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