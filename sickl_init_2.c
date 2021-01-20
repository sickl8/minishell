/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_init_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 14:33:06 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/20 19:35:18 by isaadi           ###   ########.fr       */
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

void	handle_error(int ex)
{
	exit(ex);
}

void	capture_signals(int ac, char **av)
{
	(void)ac;
	(void)av;
	signal(SIGINT, handle_signal_int);
	signal(SIGQUIT, handle_signal_quit);
}

void	reset_prompt(int jmp)
{
	if (g_sig == 0)
		OPRINTS("\b\b  \b\b");
	jmp ? OPRINTS("\n") : 0;
	if (g_sig == 0 && !g_dashc)
		init_read();
}

void	init(t_line *ref, char **envp)
{
	static char	*names[9] = { "minishell", "echo", "cd", "pwd", "export",
	"unset", "env", "exit", NULL };
	static char *b_errors[N_B_ERROR];

	b_errors[EB_UNSET_EXPORT_NVI] = "not a valid identifier";
	b_errors[EB_CD_HNT] = "HOME not set";
	b_errors[EB_CD_ONT] = "OLDPWD not set";
	b_errors[EB_CD_EXIT_TMA] = "too many arguments";
	b_errors[EB_EXIT_NAR] = "numeric argument required";
	b_errors[N_B_ERROR - 1] = NULL;
	g_bash_command = names;
	g_builtin_error = b_errors;
	init_globals(ref);
	init_line();
	init_buf();
	init_envp(envp);
	init_env();
	init_shlvl();
}
