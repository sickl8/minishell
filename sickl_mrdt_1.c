/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/18 13:01:14 by isaadi           ###   ########.fr       */
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
	char	c;

	l = sign > 0 ? "9223372036854775807" : "9223372036854775808";
	len = ft_strlen(s);
	nlen = 0;
	while (s[nlen] && s[nlen])
		nlen++;
	c = s[nlen];
	s[nlen] = '\0';
	if (nlen > 19 || (nlen == 19 && CMP(s, l) > 0))
	{
		s[nlen] = c;
		return (1);
	}
	return (0);
}

int			case_cmp(char *s1, char *s2)
{
	while (*s1 && (*s1 == *s2 || (ft_isalpha(*s1) && ft_isalpha(*s2) &&
	ft_tolower(*s1) == ft_tolower(*s2))))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

void	init_shlvl(void)
{
	// t_evar	*var;
	
	// var = find_env_p("SHLVL");
	// if (var)
	// {
	// 	free_and_set_to_null(&var->value);
	// 	if (!(var->value = ft_itoa(1)))
	// 		0;
	// }
	// else
	// {
	// 	0;
	// }
}
