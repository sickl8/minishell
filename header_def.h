/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_def.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:06:51 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/21 19:26:31 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_DEF_H
# define HEADER_DEF_H

# define YLL "9223372036854775809"
# define SLEN ft_strlen
# define GCL g_cmds_length
# define GPR g_program_return
# define AS assign
# define CASE_CMP case_cmp
# define WRITE2BUF ft_strncpy
# define MALLOC mallok
# define BPRINT bprint
# define BPRINTS bprints
# define BPRINTC bprintc
# define OPRINTS oprints
# define OPRINT oprint
# define OPRINTC oprintc
# define EPRINT eprint
# define EPRINTS eprints
# define EPRINTC eprintc
# define BASHSYN bashsyn
# define CMP cmp
# define NCMP ncmp
# define IS_REDIR is_redir
# define ISNUM isnum
# define STR_IS_REDIR str_is_redir
# define STR_IS_RL str_is_rl
# define STR_IS_RRR str_is_rrr
# define STR_IS_RR str_is_rr
# define STRCPY stgcpy
# define LEN len_
# define LENX lenx_
# define DUP ft_strdup

# define TRNCT 1024
# define APPND 9
# define PERM 0644
# define ENV_NAME_LEN_MAX 32760
# define WHTSPC 'W'
# define NONLIT 'N'
# define ENVVAR 'V'
# define SEMILIT 'S'
# define LITERAL 'L'
# define RDRWORD 'R'
# define ARG_MAX 262144
# define RL 2
# define RR 1
# define RRR 0
# define EXIT 1
# define RETURN 0
# define N_B_ERROR 6

# include "header_bufferedio.h"
# include "header_colors.h"
# include "header_commandid.h"

# define PV(x, y) printf("%s = " y, #x, x)

#endif
