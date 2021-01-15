/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:53:04 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 18:08:43 by isaadi           ###   ########.fr       */
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
#include "header_global.h"
#include "header_errors.h"

#include "header_get_next_line.h"

#include <stdio.h>

int		format_string(void)
{
	char	*tmp;

	g_bash_errno = 0;
	if ((tmp = ft_strchr(g_line->rd.buf, '\n')) != NULL)
		*tmp = '\0';
	g_line->rd_len = ft_strlen(g_line->rd.buf);
	if (!(MALLOC(&(g_line->rd.msk), g_line->rd_len + 1)))
		cleanup(EXIT);
	set_mask();
	if (g_bash_errno || !initial_error_check())
		return (free_and_set_to_null(&g_line->rd.msk) * 0 + g_bash_errno);
	split_wmask(&g_line->rd, &g_line->it, ';');
	free_buf_and_mask(g_line->rd);
	exec(NULL, 0, -1);
	free_and_set_to_null(&g_line->it);
	return (0);
}

void	exit_the_shell(void)
{
	OPRINTS("exit\n");
	cleanup(RETURN);
	exit(0);
}

void	ctrl_d(int *bk)
{
	reset_stdin(bk);
	OPRINTS("\e[2K");
	OPRINTS("\r");
}

void	init_read(void)
{
	t_evar	home;
	t_evar	user;
	t_evar	return_status;

	home = find_env("HOME");
	return_status = find_env("?");
	g_pwd.name = "PWD";
	g_pwd.value = getcwd(NULL, 0);
	g_pwd.value_len = ft_strlen(g_pwd.value);
	user = find_env("USER");
	ft_memset(g_bash_error, '\0', ARG_MAX + 2);
	g_bash_errno = 0;
	g_bash_commandid = 0;
	skittles(user.value);
	skittles("@minishell");
	BPRINTS(ESC_RESET ":") * 0 + 1 && BPRINTS(ESC_BLUE_B);
	if (ft_strstr(g_pwd.value, home.name ? home.value : NULL) == g_pwd.value)
		BPRINTS("~") * 0 + 1 && BPRINT(g_pwd.value + home.value_len);
	else
		BPRINT(g_pwd.value);
	BPRINTS(ESC_RESET "$ ");
	bflush(STDOUT_FILENO);
	free(g_pwd.value);
	free(g_line->rd.buf);
	g_line->rd.buf = NULL;
}

int		main(int ac, char **av, char **envp)
{
	t_line	line;
	int		stdin_bak;

	capture_signals(ac, av);
	init(&line, envp);
	backup_stdin(&stdin_bak);
	while (1)
	{
		if (ac > 1 && !CMP(av[1], "-c"))
			exec_from_av(ac, av);
		init_read();
		line.rd_ret = get_next_line(&line.rd.buf);
		if (line.rd_ret < 0)
			handle_error(1);
		else if (!ft_strchr(line.rd.buf, '\n') && line.rd_ret)
			ctrl_d(&stdin_bak);
		else if (line.rd_ret == 0)
			exit_the_shell();
		else
		{
			if (format_string())
				bash_error();
			free_and_set_to_null(&g_line->rd.buf);
		}
	}
}
