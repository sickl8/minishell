/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:59:55 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/18 18:44:11 by aamzouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

# include "typedef.h"

void		backslash(size_t *ref, char lit_type);
void		handle_error();
void		backup_stdin(int *bk);
t_evar		find_env(char *s);
t_evar		find_env_in_line(size_t *ref);
int			cleanup(int ex);
void		init_read();
void		split_wmask(t_bm *rd, t_bm **wr, char c);
void		free_and_set_to_null(void *adr);
int			bwrite(int fd, void* buffer, size_t len);
int			bflush(int fd);
char		**ft_msplit_whitespace(t_bm line);
t_bm		next_word(t_bm rd);
size_t		true_len(t_bm p);
void		free_buf_and_mask(t_bm s);
t_bm		previous_word(t_bm rd, t_bm ref);
void		loop_in_data();
void		bash_error();
void		free_path(char **paths);
int			*count_cmds(t_cmd *data, int *i);
void		execute_cmd(t_cmd *data, int *pfd, int j);
void		make_a_redirection(t_rdr *redir);
char		*fix_path(char **paths, int i);
void		ft_strcpy(char *dst, char *src);
int			bc_pwd(t_cmd *data);
int			bc_echo(t_cmd *data);
int			bc_env(void);
int			bc_export(t_cmd *data);
int			is_builtin(char *str);
int			builtin(t_cmd *data, int cmd);
int			bc_unset(t_cmd *data);
void		free_envar();
int			env_var_comp(char *s);
int			c_env_var_comp(char c);
int			bc_cd(t_cmd *data);
int			count_args(char **args);
int			check_var_name(char *name, int end);
void		duplicated_variable(char *arg_name, int len);
int			*check_errors_of_args(char **args, int len, int i, int j);
t_evar		ft_realloc(char *name, char *value);
t_export	calc_lengths(int *valid, int len);
char		*name_or_value(int sign, char *arg);
int			bc_exit(char **args);

#endif
