/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/23 19:25:29 by isaadi           ###   ########.fr       */
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

char	*spl_mst(char *msk, char *buf, char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (is_ws(buf[i]) && c == NONLIT)
			msk[i] = WHTSPC;
		else
			msk[i] = c;
		i++;
	}
	return (msk);
}

void	e_multiline(void)
{
	if (g_mlt == '|')
		EPRINTS("minishell: multiline not supported");
	else
	{
		EPRINTS("minishell: unexpected EOF while looking for matching `");
		EPRINTC(g_mlt);
		EPRINTC('\'');
	}
}

int		eval(char *con)
{
	return (!(!con));
}
