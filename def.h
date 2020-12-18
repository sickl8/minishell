/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:06:51 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/14 18:24:02 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_H
# define DEF_H

# define MALLOC(x, y) x = malloc(sizeof(*(x)) * (y))
# define ENV_NAME_LEN_MAX 32760
# define WHTSPC 'W'
# define NONLIT 'N'
# define ENVVAR 'V'
# define SEMILIT 'S'
# define LITERAL 'L'
# define RDRWORD 'R'
# define BASHSYN(x) (x == '|' || x == '>' || x == '<' || x == ';')
# define ARG_MAX 262144
# define CMP(x, y) (ft_strcmp(x, y))
# define NCMP(x, y, n) (ft_strncmp(x, y, n))

# define BPRINTS(x) bwrite(STDOUT_FILENO, x, sizeof(x) - 1)
# define BPRINT(x) bwrite(STDOUT_FILENO, x, ft_strlen(x))
# define BPRINTC(x) bwrite(STDOUT_FILENO, &x, 1)

# define OPRINTS(x) write(STDOUT_FILENO, x, sizeof(x))
# define OPRINT(x) write(STDOUT_FILENO, x, ft_strlen(x))
# define OPRINTC(x) write(STDOUT_FILENO, &x, 1)
# define EPRINTS(x) write(STDERR_FILENO, x, sizeof(x))
# define EPRINT(x) write(STDERR_FILENO, x, ft_strlen(x))
# define EPRINTC(x) write(STDERR_FILENO, &x, 1)
# define H printf("\nline: %d\n", __LINE__)
# define BWR(fd, string) bwrite(fd, string, ft_strlen(string))
# define IS_REDIR(x) (x == '<' || x == '>')
# define STR_IS_REDIR(x) (STR_IS_RRR(x) || STR_IS_RR(x) || STR_IS_RL(x))
# define STR_IS_RRR(x) (!NCMP(x.msk, "22", 2) && !NCMP(x.buf, ">>", 2))
# define STR_IS_RR(x) (!NCMP(x.buf, x.msk, 1) && !NCMP(x.buf, ">", 1))
# define STR_IS_RL(x) (!NCMP(x.buf, x.msk, 1) && !NCMP(x.buf, "<", 1))
# define RL 2
# define RR 1
# define RRR 0
# define EXIT 1
# define RETURN 0
# define A(x, y) assign(&x, y, sizeof(x))
# define I *ref
# define IPP (*ref)++
# define PPI ++(*ref)
# define PV(x, y) printf(#x " = " y, x)
# define APPND O_WRONLY | O_CREAT | O_APPEND, mode
# define TRNCT O_WRONLY | O_CREAT | O_TRUNC, mode
# define N_B_ERROR 5
# define STRCPY(dest, src) ft_strncpy(dest, src, -1)
# define ISNUM(x) (x >= '0' && x <= '9')
// #define g_bash_errno *get_g_bash_errno(__func__)

# include "bufferedio.h"
# include "colors.h"
# include "cursor.h"
# include "commandid.h"

#endif
