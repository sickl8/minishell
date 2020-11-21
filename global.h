/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 14:23:54 by isaadi            #+#    #+#             */
/*   Updated: 2020/11/21 18:00:51 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H

# include "typedef.h"

t_bw	g_bw;
t_line	*g_line;
// int		g_g_bash_errno;
int		g_bash_errno;
int		g_dollar_qm;
char	*g_bash_error;
char	**g_bash_command;
int		g_bash_commandid;
t_fnl	*g_list_of_commands;

#endif