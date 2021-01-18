/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/18 16:58:27 by isaadi           ###   ########.fr       */
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
	t_evar	*var;
	t_cmd	data;
	int		shlvl;

	var = find_env_p("SHLVL");
	if (var)
	{
		shlvl = ft_atoi(var->value ? var->value : "0") + 1;
		if (shlvl < 0)
			shlvl = 0;
		else if (shlvl == 1000)
			shlvl = -1;
		else if (shlvl > 1000)
			shlvl = shlvl_error(shlvl);
		free_and_set_to_null(&var->value);
		if (!(var->value = (shlvl != -1 ? ft_itoa(shlvl) : ft_strdup(""))))
			cleanup(EXIT);
	}
	else
	{
		data.args = (char*[]){ "export", "SHLVL=1", NULL };
		g_cmds_length = 1;
		bc_export_bk(&data);
		g_cmds_length = 0;
	}
}

int		shlvl_error(int shlvl)
{
	char	*num;
	size_t	len;

	g_bash_errno = E_WARNING;
	ft_strncpy(g_bash_error, "shell level (", sizeof("shell level (") - 1);
	if (!(num = ft_itoa(shlvl)))
		cleanup(EXIT);	
	ft_strncpy(g_bash_error + sizeof("shell level (") - 1, num, ft_strlen(num));
	ft_strncpy(g_bash_error + sizeof("shell level (") - 1 + ft_strlen(num),
	") too high, resetting to 1", sizeof(") too high, resetting to 1") - 1);
	len = sizeof("shell level (") - 1 + ft_strlen(num) + 
	sizeof(") too high, resetting to 1") - 1;
	g_bash_error[len] = '\0';
	bash_error();
}
