/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sickl_error_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:26:57 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/17 16:38:37 by isaadi           ###   ########.fr       */
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

void	e_builtin(void)
{
	EPRINTS("minishell: ");
	EPRINT(g_bash_command[g_bash_commandid]);
	EPRINTS(": ");
	if (g_builtin_errno != EB_CD_HNT && g_builtin_errno != EB_CD_EXIT_TMA)
	{
		// EPRINTS("`");
		EPRINT(g_bash_error);
		// EPRINTS("'");
		EPRINTS(": ");
	}
	if (g_builtin_errno != -1)
		EPRINT(g_builtin_error[g_builtin_errno]);
	else
		EPRINT(strerror(errno));
}

void	e_errno(void)
{
	EPRINT(g_bash_command[g_bash_commandid]);
	EPRINTS(": ");
	EPRINT(g_bash_error);
	EPRINTS(": ");
	EPRINT(strerror(errno));
}

void	e_command(void)
{
	EPRINTS("minishell: ");
	EPRINT(g_bash_error);
	EPRINTS(": command not found");
}

void	e_syntax(void)
{
	EPRINTS("minishell: syntax error near unexpected token `");
	EPRINT(g_bash_error);
	EPRINTS("'");
}

void	bash_error(void)
{
	if (g_bash_errno == E_SYNTAX)
		e_syntax();
	else if (g_bash_errno == E_MULTILINE)
		EPRINTS("minishell: multiline is not supported");
	else if (g_bash_errno == E_COMMAND)
		e_command();
	else if (g_bash_errno == E_ERRNO)
		e_errno();
	else if (g_bash_errno == E_BUILTIN)
		e_builtin();
	EPRINTS("\n");
}
