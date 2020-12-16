/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:26:44 by isaadi            #+#    #+#             */
/*   Updated: 2020/11/27 18:52:10 by aamzouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define E_ERRNO 1
# define E_BUILTIN 2
# define E_MULTILINE 3
# define E_CNTEXEC 126
# define E_COMMAND 127
# define E_SIG(x) 128 + x
# define E_SYNTAX 258
# define EB_UNSET_EXPORT_NVI 0

#endif
