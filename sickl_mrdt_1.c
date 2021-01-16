/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/16 17:56:25 by isaadi           ###   ########.fr       */
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

int			continue_check_num(char *s, int sign)
{
	char	*l;
	size_t	len;
	size_t	nlen;

	l = sign > 0 ? "9223372036854775807" : "9223372036854775808";
	len = ft_strlen(s);
	nlen = 0;
	while (s[nlen] && s[nlen])
		nlen++;
	s[nlen] = '\0';
	if (nlen > 19 || (nlen == 19 && CMP(s, l) > 0))
		return (1);
	return (0);
}
