/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_macros_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 19:05:02 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/13 16:38:14 by isaadi           ###   ########.fr       */
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
#include "proto.h"
#include "typedef.h"
#include "def.h"
#include "extern.h"
#include "errors.h"

#include "get_next_line.h"

#include <stdio.h>

size_t	_lenx(void *p, int sz, size_t term)
{
	return (ft_len(p, sz, term));
}

size_t	_len(void *p, int sz)
{
	return (ft_len(p, sz, 0));
}