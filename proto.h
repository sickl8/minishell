/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:59:55 by isaadi            #+#    #+#             */
/*   Updated: 2020/11/10 16:58:40 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

# include "typedef.h"

void	backslash(size_t *ref, char lit_type);
void	handle_error();
void	backup_stdin(int *bk);
t_evar	find_env(char *s);
t_evar	find_env_in_line(size_t *ref);
int		cleanup(int ex);
void	init_read();
void	split_wmask(t_bm *rd, t_bm **wr, char c);
void	free_and_set_to_null(void *adr);
int		bwrite(int fd, void* buffer, size_t len);
int		bflush(int fd);
char	**ft_msplit_whitespace(t_bm line);
t_bm	next_word(t_bm rd);
size_t	true_len(t_bm p);
void	free_buf_and_mask(t_bm s);
t_bm	previous_word(t_bm rd, t_bm ref);

#endif
