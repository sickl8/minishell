/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_init_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 14:33:06 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 16:43:16 by isaadi           ###   ########.fr       */
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

void	continue_init_env(void)
{
	int		i;

	i = -1;
	while (g_line->envp[++i])
	{
		g_line->env_var[i].name_len =
		ft_strchr(g_line->envp[i], '=') - g_line->envp[i];
		if (!(MALLOC(&(g_line->env_var[i].name),
		g_line->env_var[i].name_len + 1)))
			cleanup(EXIT);
		g_line->env_var[i].name[g_line->env_var[i].name_len] = '\0';
		ft_strncpy(g_line->env_var[i].name,
		g_line->envp[i], g_line->env_var[i].name_len);
		if (!(g_line->env_var[i].value =
		ft_strdup(ft_strchr(g_line->envp[i], '=') + 1)))
			cleanup(EXIT);
		g_line->env_var[i].value_len = ft_strlen(g_line->env_var[i].value);
	}
	set_name_only(0, NULL, 0, NULL);
}

void	handle_signal_quit(int sig)
{
	(void)sig;
	OPRINTS("\e[2K");
	OPRINTS("\r");
	reset_prompt(0);
}

void	handle_signal_int(int sig)
{
	(void)sig;
	reset_prompt(1);
	g_program_return = 1;
}

void	print_color(int color)
{
	if (color == 0)
		BPRINTS(ESC_RED_B);
	else if (color == 1)
		BPRINTS(ESC_ORANGE_B);
	else if (color == 2)
		BPRINTS(ESC_YELLOW_B);
	else if (color == 3)
		BPRINTS(ESC_LIME_B);
	else if (color == 4)
		BPRINTS(ESC_GREEN_B);
	else if (color == 5)
		BPRINTS(ESC_TURQUOISE_B);
	else if (color == 6)
		BPRINTS(ESC_CYAN_B);
	else if (color == 7)
		BPRINTS(ESC_BLUE_B);
	else if (color == 8)
		BPRINTS(ESC_PINK_B);
	else if (color == 9)
		BPRINTS(ESC_PURPLE_B);
	else if (color == 10)
		BPRINTS(ESC_MAGENTA_B);
}

void	skittles(char *s)
{
	static int	color;
	int			i;

	i = -1;
	while (s[++i])
	{
		print_color(color);
		BPRINTC(s[i]);
		color++;
		color %= 11;
	}
}
