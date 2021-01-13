/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_macros_0.c                                   :+:      :+:    :+:   */
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

void	*mallok(void *adr, size_t bytes)
{
	void	**cast;
	
	cast = adr;
	*cast = malloc(bytes);
	return (*cast);
}

int		bashsyn(char x)
{
	return (x == '|' || x == '>' || x == '<' || x == ';');
}

int		cmp(char *x, char *y)
{
	return (ft_strcmp(x, y));
}

int		ncmp(char *x, char *y, size_t n)
{
	return (ft_strncmp(x, y, n));
}

int		bprints(char *s)
{
	return (bwrite(STDOUT_FILENO, s, ft_strlen(s)));
}
