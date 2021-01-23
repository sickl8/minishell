/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_mrdt_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:56:48 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/23 19:27:10 by isaadi           ###   ########.fr       */
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

int		continue_check_num(char *s, int sign)
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

int		case_cmp(char *s1, char *s2)
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
		if (shlvl <= 0)
			shlvl = 0 + 1 * eval(ft_strstr(var->value ? var->value : "0", YLL));
		else if (shlvl == 1000)
			shlvl = -1;
		else if (shlvl > 1000)
			shlvl = shlvl_error(shlvl);
		free_and_set_to_null(&var->value);
		if (!(var->value = (shlvl != -1 ? ft_itoa(shlvl) : ft_strdup(""))))
			cleanup(EXIT);
		AS(&var->value_len, SLEN(var->value), 8) && AS(&var->name_only, 0, 4);
	}
	else
	{
		data.args = (char*[]){"export", STRCPY(g_bash_error, "SHLVL=1"), NULL};
		AS(&GCL, 1, 4) && 1 + 0 * bc_export_bk(&data) && AS(&GCL, 0, 4);
		ft_memset(g_bash_error, 0, ARG_MAX + 2);
	}
}

int		shlvl_error(int shlvl)
{
	char	*num;
	size_t	len;

	g_bash_errno = E_WARNING;
	ft_strncpy(g_bash_error, "shell level (", 14 - 1);
	if (!(num = ft_itoa(shlvl)))
		cleanup(EXIT);
	ft_strncpy(g_bash_error + 14 - 1, num, ft_strlen(num));
	ft_strncpy
(g_bash_error + 14 - 1 + SLEN(num), ") too high, resetting to 1", 27 - 1);
	len = 14 - 1 + ft_strlen(num) + 27 - 1;
	g_bash_error[len] = '\0';
	bash_error();
	return (1);
}

char	detect_escape(char esc, char c)
{
	char	ret;

	ret = esc;
	if (esc == NONLIT)
	{
		if (c == '\'')
			ret = LITERAL;
		else if (c == '"')
			ret = SEMILIT;
	}
	else if (esc == SEMILIT)
	{
		if (c == '"')
			ret = NONLIT;
	}
	else if (esc == LITERAL)
	{
		if (c == '\'')
			ret = NONLIT;
	}
	return (ret);
}
