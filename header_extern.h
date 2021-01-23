/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_extern.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 14:23:54 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/20 16:51:13 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_EXTERN_H
# define HEADER_EXTERN_H

# include "header_typedef.h"

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
extern void		*g_ez;
extern char		g_mlt;
extern int		g_dashc;

#endif
