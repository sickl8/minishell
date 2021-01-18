/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_macros_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 19:05:02 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/18 17:50:18 by isaadi           ###   ########.fr       */
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

int		str_is_rl(t_bm bm)
{
	return (!ncmp(bm.buf, bm.msk, 1) && !ncmp(bm.buf, "<", 1));
}

int		str_is_rr(t_bm bm)
{
	return (!ncmp(bm.buf, bm.msk, 1) && !ncmp(bm.buf, ">", 1));
}

int		str_is_rrr(t_bm bm)
{
	return (!ncmp(bm.msk, "22", 2) && !ncmp(bm.buf, ">>", 2));
}

int		str_is_redir(t_bm bm)
{
	return (str_is_rrr(bm) || str_is_rr(bm) || str_is_rl(bm));
}

char	*stgcpy(char *dest, char *src)
{
	return (ft_strcpy(dest, src));
}
