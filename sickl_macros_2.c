/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_macros_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 19:05:02 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/06 18:08:32 by isaadi           ###   ########.fr       */
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
#include "global.h"
#include "errors.h"

#include "get_next_line.h"

#include <stdio.h>

ssize_t	eprints(char *s)
{
	return (write(STDERR_FILENO, s, ft_strlen(s)));
}

ssize_t	eprint(char *s)
{
	return (oprints(s));
}

ssize_t	eprintc(char c)
{
	return (write(STDERR_FILENO, &c, 1));
}

int		is_redir(char c)
{
	return (c == '<' || c == '>');
}

int		isnum(char c)
{
	return (ft_isdigit(c));
}