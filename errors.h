/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:26:44 by isaadi            #+#    #+#             */
/*   Updated: 2020/11/10 18:07:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define E_ERRNO 1
# define E_SYNTAX 2
# define E_MULTILINE 3
# define E_CNTEXEC 126
# define E_COMMAND 127
# define E_SIG(x) 128 + x

#endif