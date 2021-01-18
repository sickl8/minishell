/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_proto.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:59:55 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/18 17:49:24 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_PROTO_H
# define HEADER_PROTO_H

# include "header_typedef.h"

void		backslash(size_t *ref, char lit_type);
void		handle_error(int ex);
void		backup_stdin(int *bk);
t_evar		find_env(char *s);
t_evar		find_env_in_line(size_t *ref);
int			cleanup(int ex);
void		init_read();
void		split_wmask(t_bm *rd, t_bm **wr, char c);
int			free_and_set_to_null(void *adr);
int			bwrite(int fd, void *buffer, size_t len);
int			bflush(int fd);
char		**ft_msplit_whitespace(t_bm line);
t_bm		next_word(t_bm rd);
size_t		true_len(t_bm p);
void		free_buf_and_mask(t_bm s);
t_bm		previous_word(t_bm rd, t_bm ref);
void		loop_in_data();
void		bash_error();
void		free_path(char **paths);
int			*count_cmds(t_cmd *data);
void		execute_cmd(t_cmd *data, int *pfd, int j);
void		make_a_redirection(t_rdr *redir);
char		*fix_path(char **paths, int i);
char		*ft_strcpy(char *dst, char *src);
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
t_evar		name_or_value(char *arg);
int			bc_exit(char **args);
void		free_tmp();
int			go_to(t_cmd *data);
int			change_dir(char *s, char *cwd);
int			assign(void *p, unsigned long long v, int size);
int			eerf(void *p);
int			bc_cd_fork(t_cmd *data);
int			go_to_fork(t_cmd *data);
size_t		ft_len(void *p, int	size, size_t end);
void		*failing_error(t_cmd *data);
int			print_all_envs(t_cmd *data);
void		*mallok(void *adr, size_t bytes);
int			bashsyn(char x);
int			cmp(char *x, char *y);
int			ncmp(char *x, char *y, size_t n);
int			bprints(char *s);
int			bprint(char *s);
int			bprintc(char c);
ssize_t		oprints(char *s);
ssize_t		oprint(char *s);
ssize_t		oprintc(char c);
ssize_t		eprints(char *s);
ssize_t		eprint(char *s);
ssize_t		eprintc(char c);
int			is_redir(char c);
int			isnum(char c);
int			str_is_rl(t_bm bm);
int			str_is_rr(t_bm bm);
int			str_is_rrr(t_bm bm);
int			str_is_redir(t_bm bm);
char		*stgcpy(char *dest, char *src);
size_t		lenx_(void *p, int sz, size_t term);
size_t		len_(void *p, int sz);
void		set_zero(t_evar *tmp, int end);
void		set_name_only(int start, int *valid, int len, t_evar *tmp);
int			assign_valid_args_bk(int argc, char **argv, t_evar *tmp);
int			free_tmp_ava(t_evar *tmp);
char		*reset_evar(t_evar *var);
t_evar		get_evar(char *s);
char		**sanitize_av(int ac, char **av, int *ret);
void		assign_old_args(int x, t_evar *tmp);
int			valid_arg(int ac, char **av, int index);
int			bc_export_bk(t_cmd *data);
t_evar		*find_env_p(char *s);
void		invalid_arg(char **av, int i, int *rt);
void		export_new_vars(char **args, int i, t_evar *tmp, int j);
void		export_old_vars(char **args, t_export len, int *valid, t_evar *tmp);
void		exec(t_fnl **tracer, int i, int x);
void		bash_error(void);
int			check_s_bashsyn(t_bm p);
int			check_s_redir(t_bm p);
int			check_syntax(void);
int			count_quote(char c);
int			check_multiline(void);
void		continue_get_cmd_p_args(char **ret, t_bm *redir, t_rdr **rdr);
char		**get_cmd_p_args(t_bm *redir, t_rdr **rdr);
void		set_cmd_2_null(t_cmd *t);
t_cmd		*get_cmd(t_bm **redir);
int			initial_error_check(void);
int			assign(void *p, unsigned long long v, int size);
void		copy_valid_chars(char *dst, char *src, char *msk, size_t tr_ln);
size_t		true_len(t_bm p);
void		continue_split_redirects(t_bm *rd, t_bm **wr, size_t wc);
t_bm		previous_word(t_bm rd, t_bm ref);
t_bm		next_word(t_bm rd);
size_t		count_words_redir(t_bm *rd);
void		split_redirect(t_bm *rd, t_bm **wr);
void		split_redirects(void);
void		split_pipe(void);
void		continue_split_wmask(t_bm *rd, t_bm **wr, size_t cnt, char c);
void		split_wmask(t_bm *rd, t_bm **wr, char c);
void		reset_stdin(int *bk);
void		backup_stdin(int *bk);
void		continue_rplc_env_var(void);
t_evar		find_env_wv(char *s);
t_evar		*find_env_p(char *s);
t_evar		find_env(char *s);
t_evar		find_env_in_line(size_t *ref);
void		rplc_env_var(void);
int			is_ws(char c);
int			env_var_comp(char *s);
int			c_env_var_comp(char c);
void		env_var(size_t *ref);
void		bash_syntax(size_t *ref);
void		squote(size_t *ref);
void		dquote(size_t *ref);
void		backslash(size_t *ref, char lit_type);
int			set_mask(void);
void		free_loc(void);
void		free_envp(void);
void		free_redir(void);
void		free_g_bw(void);
void		free_pipe(void);
void		free_scol(void);
void		free_envar(void);
int			cleanup(int ex);
void		free_it(void);
void		fastn(void);
void		free_buf_and_mask(t_bm s);
void		free_tmp(void);
int			format_string(void);
void		reset_prompt(int jmp);
void		exit_the_shell(void);
void		ctrl_d(int *bk);
void		handle_signal(int sig);
void		handle_error(int ex);
void		print_color(int color);
void		skittles(char *s);
void		init_read(void);
void		continue_init_env(void);
void		init_env(void);
void		init_envp(char **envp);
void		init_buf(void);
void		init_line(void);
void		init_globals(t_line *ref);
void		init(t_line *ref, char **envp);
void		capture_signals(int ac, char **av);
void		handle_signal_int(int sig);
void		handle_signal_quit(int sig);
void		exec_from_av(int ac, char **av);
void		set_pwd(void);
int			continue_check_num(char *s, int sign);
int			*check_errors_of_unset(char **args, int len, int i, int j);
char		**assign_valid_args(char **args, int *valid, int len);
int			check_echo_option(t_cmd *data);
int			case_cmp(char *s1, char *s2);
void		e_warning(void);
void		init_shlvl(void);
int			shlvl_error(int shlvl);

#endif
