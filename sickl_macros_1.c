/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_macros_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 19:05:02 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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

int		bprint(char *s)
{
	return (bprints(s));
}

int		bprintc(char c)
{
	return (bwrite(STDOUT_FILENO, &c, 1));
}

ssize_t	oprints(char *s)
{
	return (write(STDOUT_FILENO, s, ft_strlen(s)));
}

ssize_t	oprint(char *s)
{
	return (oprints(s));
}

ssize_t	oprintc(char c)
{
	return (write(STDOUT_FILENO, &c, 1));
}
