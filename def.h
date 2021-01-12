/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:06:51 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/11 19:33:56 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_H
# define DEF_H

// # define MALLOC(x, y) x = malloc(sizeof(*(x)) * (y))
// # define BPRINTS(x) bwrite(STDOUT_FILENO, x, sizeof(x) - 1)
// # define BPRINT(x) bwrite(STDOUT_FILENO, x, ft_strlen(x))
// # define BPRINTC(x) bwrite(STDOUT_FILENO, &x, 1)
// # define BASHSYN(x) (x == '|' || x == '>' || x == '<' || x == ';')
// # define CMP(x, y) (ft_strncmp(x, y))
// # define NCMP(x, y, n) (ft_strncmp(x, y, n))
// # define OPRINTS(x) write(STDOUT_FILENO, x, sizeof(x) - 1)
// # define OPRINT(x) write(STDOUT_FILENO, x, ft_strlen(x))
// # define OPRINTC(x) write(STDOUT_FILENO, &x, 1)
// # define EPRINTS(x) write(STDERR_FILENO, x, sizeof(x) - 1)
// # define EPRINT(x) write(STDERR_FILENO, x, ft_strlen(x))
// # define EPRINTC(x) write(STDERR_FILENO, &x, 1)
// # define BWR(fd, string) bwrite(fd, string, ft_strlen(string))
// # define IS_REDIR(x) (x == '<' || x == '>')
// # define A(x, y) assign(&x, y, sizeof(x))
// # define ISNUM(x) (x >= '0' && x <= '9')
// # define STR_IS_REDIR(x) (STR_IS_RRR(x) || STR_IS_RR(x) || STR_IS_RL(x))
// # define STR_IS_RRR(x) (!NCMP(x.msk, "22", 2) && !NCMP(x.buf, ">>", 2))
// # define STR_IS_RR(x) (!NCMP(x.buf, x.msk, 1) && !NCMP(x.buf, ">", 1))
// # define STR_IS_RL(x) (!NCMP(x.buf, x.msk, 1) && !NCMP(x.buf, "<", 1))
// # define STRCPY(dest, src) ft_strncpy(dest, src, -1)
// # define LEN(x) ft_len(x, sizeof(x), 0)
// # define LENX(x, y) ft_len(x, sizeof(x), y)

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
# define LEN _len
# define LENX _lenx
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

# include "bufferedio.h"
# include "colors.h"
# include "cursor.h"
# include "commandid.h"

// # define H printf("\nline: %d\n", __LINE__)
# define PV(x, y) printf("%s = " y, #x, x)
# define EPV(x, y) fprintf(stderr, "%s = " y, #x, x)
#include <time.h>
#define TIME(x) b = clock(); x printf("The section : \"%.20s\"[...] took %lfs = %lfms\n", #x, (double)(clock() - b) / CLOCKS_PER_SEC, ((double)(clock() - b) / CLOCKS_PER_SEC) * 1000)
#define TEST_FORK() TIME(if (!fork()) exit(0);)

#endif
