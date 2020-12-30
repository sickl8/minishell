/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickl8 <sickl8@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:26:44 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/30 15:17:04 by sickl8           ###   ########.fr       */
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
# define EB_CD_HNT 1
# define EB_CD_EXIT_TMA 2
# define EB_EXIT_NAR 3
# define EB_CD_ONT 4

#endif
