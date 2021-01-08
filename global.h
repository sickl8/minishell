/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 14:23:54 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/08 14:53:52 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H

# include "typedef.h"

t_bw	g_bw;
t_line	*g_line;
// int		g_g_bash_errno;
int		g_bash_errno;
char	*g_bash_error;
int		g_dollar_qm;
char	**g_bash_command;
int		g_builtin_errno;
char	**g_builtin_error;
int		g_bash_commandid;
t_fnl	*g_list_of_commands;
int		g_program_return;
// int		g_g_program_return;
char	g_s_program_return[100];
int		g_sig;
pid_t	g_pid;
int		g_dup;
int     g_cmds_length;
t_evar	g_pwd;
int		g_parent;
clock_t b;

#endif
