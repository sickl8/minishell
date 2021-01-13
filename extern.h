/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 14:23:54 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/13 16:34:44 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTERN_H
# define EXTERN_H

# include "typedef.h"

// extern int		g_g_bash_errno;
// extern int		g_g_program_return;
extern t_bw		g_bw;
extern t_line	*g_line;
extern int		g_bash_errno;
extern char		*g_bash_error;
extern int		g_dollar_qm;
extern char		**g_bash_command;
extern int		g_builtin_errno;
extern char		**g_builtin_error;
extern int		g_bash_commandid;
extern t_fnl	*g_list_of_commands;
extern int		g_program_return;
extern char		g_s_program_return[100];
extern int		g_sig;
extern pid_t	g_pid;
extern int		g_dup;
extern int		g_cmds_length;
extern t_evar	g_pwd;
extern int		g_parent;
extern clock_t	b;
extern void		*g_ez;

#endif
