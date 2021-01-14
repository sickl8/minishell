/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 12:53:04 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "header_proto.h"
#include "header_typedef.h"
#include "header_def.h"
#include "header_global.h"
#include "header_errors.h"

#include "header_get_next_line.h"

#include <stdio.h>


void	exec(t_fnl **tracer, int i, int x)
{
	while (g_line->it[++x].buf)
	{
		g_line->rd = g_line->it[x];
		rplc_env_var();
		split_wmask(&g_line->env, &g_line->scol, ';');
		if (!(MALLOC(&(g_line->pipe), 8 * g_line->env.cnt + 8)))
			cleanup(EXIT);
		g_line->pipe[g_line->env.cnt] = NULL;
		split_pipe();
		split_redirects();
		i = -1;
		g_list_of_commands = NULL;
		tracer = &g_list_of_commands;
		while (++i < (int)g_line->env.cnt)
		{
			if (!(MALLOC(&(*tracer), 16)))
				cleanup(EXIT);
			(*tracer)->cmd_and_args = NULL;
			(*tracer)->next = NULL;
			(*tracer)->cmd_and_args = get_cmd(g_line->redir[i]);
			tracer = &(*tracer)->next;
		}
		loop_in_data();
		free_tmp();
	}
}

int		check_s_bashsyn(t_bm p)
{
	t_bm	tmp;

	tmp = p;
	tmp = previous_word(tmp, g_line->rd);
	if (!tmp.msk || tmp.msk[0] == ';' || tmp.msk[0] == '|')
	{
		g_bash_errno = E_SYNTAX;
		if (!tmp.msk)
			ft_strncpy(g_bash_error, p.msk, 1);
		else
			ft_strncpy(g_bash_error, tmp.msk, 1);
		return (1);
	}
	return (0);
}

int		check_s_redir(t_bm p)
{
	t_bm	tmp;

	tmp = p;
	tmp.buf += tmp.cnt;
	tmp.msk += tmp.cnt;
	tmp = next_word(tmp);
	if (!tmp.msk || STR_IS_REDIR(tmp) || BASHSYN(tmp.msk[0]))
	{
		g_bash_errno = E_SYNTAX;
		if (!tmp.msk)
			ft_strncpy(g_bash_error, "newline", 7);
		else
			ft_strncpy(g_bash_error, tmp.buf, tmp.cnt);
		return (1);
	}
	return (0);
}

int		check_syntax()
{
	t_bm	p;

	p = next_word(g_line->rd);
	while (p.msk)
	{
		if (STR_IS_REDIR(p) && check_s_redir(p))
			return (1);
		else if (!STR_IS_REDIR(p) && BASHSYN(p.msk[0]) && check_s_bashsyn(p))
			return (1);
		p.buf += p.cnt;
		p.msk += p.cnt;
		p = next_word(p);
	}
	return (0);
}

int		count_quote(char c)
{
	int		ret;
	char	*p;

	ret = 0;
	p = ft_strchr(g_line->rd.msk, c);
	while (p)
	{
		ret++;
		p = ft_strchr(++p, c);
	}
	return (ret);
}

int		check_multiline()
{
	char	*p;

	p = ft_strrchr(g_line->rd.msk, '|');
	if (p)
		while (*(++p))
			if (*p != WHTSPC)
				break ;
	if (count_quote('\'') % 2 || count_quote('"') % 2 || (g_line->rd_len > 0 &&
	g_line->rd.buf[g_line->rd_len - 1] == '\\' &&
	g_line->rd.msk[g_line->rd_len - 1] == '\\') || (p && !*p))
		return (1 + 0 * (g_bash_errno = E_MULTILINE));
	return (0);
}

void	continue_get_cmd_p_args(char **ret, t_bm *redir, t_rdr **rdr)
{
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (redir[++i].buf)
	{
		if (STR_IS_REDIR(redir[i]))
		{
			rdr[0][j].file_name = redir[i + 1].buf;
			if (STR_IS_RL(redir[i]))
				rdr[0][j].type = RL;
			else if (STR_IS_RR(redir[i]))
				rdr[0][j].type = RR;
			else
				rdr[0][j].type = RRR;
			j++;
			i++;
		}
		else
			ret[i - 2 * j] = redir[i].buf;
	}
}

char	**get_cmd_p_args(t_bm *redir, t_rdr **rdr)
{
	char	**ret;
	int		cnt;
	int		total;

	cnt = 0;
	total = -1;
	while (redir[++total].buf)
		if (STR_IS_REDIR(redir[total]))
			cnt++;
	if (!(MALLOC(&(ret), 8 * (total - 2 * cnt + 1))) ||
	!(MALLOC(&(*rdr), 16 * (cnt + 1))))
		cleanup(EXIT);
	ret[total - 2 * cnt] = NULL;
	rdr[0][cnt].file_name = NULL;
	continue_get_cmd_p_args(ret, redir, rdr);
	return (ret);
}

void	set_cmd_2_null(t_cmd *t)
{
	t->cpa = NULL;
	t->find = NULL;
	t->args = NULL;
	t->path2exec = NULL;
}

t_cmd	*get_cmd(t_bm **redir)
{
	t_cmd	*ret;
	t_cmd	**tracer;
	int		i;

	i = -1;
	tracer = &ret;
	while (redir[++i])
	{
		if (!(MALLOC(&(*tracer), 56)))
			cleanup(EXIT);
		set_cmd_2_null(*tracer);
		(*tracer)->next = NULL;
		(*tracer)->fd_read = 0;
		(*tracer)->fd_write = 1;
		(*tracer)->cpa = get_cmd_p_args(redir[i], &((*tracer)->redir));
		(*tracer)->find = (*tracer)->cpa[0];
		(*tracer)->args = (*tracer)->cpa;
		tracer = &(*tracer)->next;
	}
	return (ret);
}

int		initial_error_check()
{
	if (check_multiline() || check_syntax())
		return (0);
	return (1);
}

int		assign(void *p, unsigned long long v, int size)
{
	if (size == 1)
		*((char*)(p)) = (char)v;
	else if (size == 2)
		*((short*)(p)) = (short)v;
	else if (size == 4)
		*((int*)(p)) = (int)v;
	else if (size == 8)
		*((long*)(p)) = (long)v;
	return (1);
}

void	copy_valid_chars(char *dst, char *src, char *msk, size_t tr_ln)
{
	size_t	i;

	i = 0;
	while (src[i] && i < tr_ln)
	{
		while (src[i] && msk[i] != LITERAL && msk[i] != SEMILIT &&
		msk[i] != NONLIT && msk[i] != '<' && msk[i] != '>' && msk[i] != '2')
		{
			src++;
			msk++;
		}
		if (!src[i])
			break ;
		dst[i] = src[i];
		i++;
	}
}

size_t	true_len(t_bm p)
{
	size_t	ret;

	if ((!(NCMP(p.buf, p.msk, p.cnt)) && (IS_REDIR(p.msk[0]))) ||
	(p.cnt == 2 && !NCMP(p.msk, "22", 2) && !NCMP(p.buf, ">>", 2)))
		return (p.cnt);
	ret = 0;
	while (p.buf[0] && p.msk[0] != WHTSPC && p.msk[0] != '>' && p.msk[0] != '<')
	{
		if (p.msk[0] == SEMILIT || p.msk[0] == LITERAL || p.msk[0] == NONLIT)
			ret++;
		p.buf++;
		p.msk++;
	}
	return (ret);
}

void	continue_split_redirects(t_bm *rd, t_bm **wr, size_t wc)
{
	t_bm	p;
	size_t	tr_ln;
	size_t	i;

	i = 0;
	p = next_word(*rd);
	while (i < wc)
	{
		wr[0][i].buf = NULL;
		wr[0][i].msk = NULL;
		tr_ln = true_len(p);
		if (!(MALLOC(&(wr[0][i].buf), tr_ln + 1)))
			cleanup(EXIT);
		wr[0][i].buf[tr_ln] = '\0';
		if (!(MALLOC(&(wr[0][i].msk), tr_ln + 1)))
			cleanup(EXIT);
		wr[0][i].msk[tr_ln] = '\0';
		copy_valid_chars(wr[0][i].buf ,p.buf, p.msk, tr_ln);
		copy_valid_chars(wr[0][i].msk ,p.msk, p.msk, tr_ln);
		p.msk += p.cnt;
		p.buf += p.cnt;
		p = next_word(p);
		i++;
	}
}

t_bm	previous_word(t_bm rd, t_bm ref)
{
	while (ref.msk < (rd.msk - 1) && rd.msk[-1] == WHTSPC)
	{
		rd.msk--;
		rd.buf--;
	}
	if (ref.msk >= rd.msk || rd.msk[-1] == WHTSPC)
	{
		rd.msk = NULL;
		return (rd);
	}
	if (BASHSYN(rd.msk[-1]))
	{
		rd.cnt = 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
		return (rd);
	}
	else if (rd.msk[-1] == '2')
	{
		rd.cnt = 2 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
		return (rd);
	}
	rd.cnt = 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
	while (ref.msk < rd.msk && rd.msk[-1] != WHTSPC &&
	!BASHSYN(rd.msk[-1]))
		rd.cnt += 1 + 0 * ((size_t)rd.buf-- + (size_t)rd.msk--);
	return (rd);
}

t_bm	next_word(t_bm rd)
{
	while (rd.msk[0] == WHTSPC && rd.buf[0])
	{
		rd.msk++;
		rd.buf++;
	}
	if (BASHSYN(rd.msk[0]))
	{
		rd.cnt = 1;
		return (rd);
	}
	else if (rd.msk[0] == '2')
	{
		rd.cnt = 2;
		return (rd);
	}
	else if (!rd.buf[0])
	{
		rd.msk = NULL;
		return (rd);
	}
	rd.cnt = 0;
	while (rd.msk[rd.cnt] != WHTSPC &&
	!BASHSYN(rd.msk[rd.cnt]) && rd.buf[rd.cnt])
		rd.cnt++;
	return (rd);
}

size_t	count_words_redir(t_bm *rd)
{
	size_t	ret;
	t_bm	p;

	ret = 0;
	p = next_word(*rd);
	while (p.msk)
	{
		ret++;
		p.msk += p.cnt;
		p.buf += p.cnt;
		p = next_word(p);
	}
	return (ret);
}

void	split_redirect(t_bm *rd, t_bm **wr)
{
	size_t	wc;

	wc = count_words_redir(rd);
	if (!(MALLOC(&(*wr), 24 * wc + 24)))
		cleanup(EXIT);
	wr[0][wc].buf = NULL;
	wr[0][wc].msk = NULL;
	continue_split_redirects(rd, wr, wc);
}

void	split_redirects()
{
	size_t	i;
	size_t	j;

	if (!(MALLOC(&(g_line->redir), 8 * g_line->env.cnt + 8)))
		cleanup(EXIT);
	g_line->redir[g_line->env.cnt] = NULL;
	i = -1;
	while (++i < g_line->env.cnt)
	{
		g_line->redir[i] = NULL;
		if (!(MALLOC(&(g_line->redir[i]), 8 * g_line->scol[i].cnt + 8)))
			cleanup(EXIT);
		g_line->redir[i][g_line->scol[i].cnt] = NULL;
		j = -1;
		while (++j < g_line->scol[i].cnt)
		{
			g_line->redir[i][j] = NULL;
			split_redirect(&g_line->pipe[i][j], &g_line->redir[i][j]);
		}
	}
}

void	split_pipe()
{
	size_t	i;

	i = -1;
	while (++i < g_line->env.cnt)
	{
		g_line->pipe[i] = NULL;
		split_wmask(&g_line->scol[i], &g_line->pipe[i], '|');
	}
}

void	continue_split_wmask(t_bm *rd, t_bm **wr, size_t cnt, char c)
{
	size_t	i;
	char	*p;
	char	*bk;

	p = rd->msk;
	bk = rd->msk;
	i = -1;
	while (++i < cnt)
	{
		wr[0][i].msk = NULL;
		wr[0][i].buf = NULL;
		if (!(p = ft_strchr(p, c)))
			p = ft_strchr(rd->msk, '\0');
		if (!(MALLOC(&(wr[0][i].buf), p - bk + 1)))
			cleanup(EXIT);
		wr[0][i].buf[p - bk] = '\0';
		if (!(MALLOC(&(wr[0][i].msk), p - bk + 1)))
			cleanup(EXIT);
		wr[0][i].msk[p - bk] = '\0';
		ft_strncpy(wr[0][i].buf, rd->buf + (bk - rd->msk), p - bk);
		ft_strncpy(wr[0][i].msk, rd->msk + (bk - rd->msk), p - bk);
		bk = ++p;
	}
}

void	reset_stdin(int *bk)
{
	if (close(0) == -1)
		handle_error(1);
	if (dup2(*bk, 0) == -1)
		handle_error(1);
	if (close(*bk) == -1)
		handle_error(1);
	backup_stdin(bk);
	// free(g_line->rd.buf);
}

void	backup_stdin(int *bk)
{
	if ((*bk = dup(STDIN_FILENO)) == -1)
		handle_error(1);
}

void	continue_rplc_env_var()
{
	size_t	i;
	size_t	j;
	t_evar	env;

	i = -1;
	j = 0;
	while (g_line->rd.buf[++i])
	{
		if (g_line->rd.msk[i] == '$')
		{
			env = find_env_in_line(&i);
			ft_strncpy(&g_line->env.buf[j], env.value, env.value_len);
			ft_memset(&g_line->env.msk[j], LITERAL, env.value_len);
			j += env.value_len;
		}
		else
		{
			g_line->env.buf[j] = g_line->rd.buf[i];
			g_line->env.msk[j] = g_line->rd.msk[i];
			j++;
		}
	}
}

t_evar	find_env_wv(char *s)
{
	size_t	j;
	t_evar	ret;
	char	*tmp;

	j = 0;
	if (!CMP(s, "?"))
	{
		ret.name = s;
		ret.name_len = 1;
		if (!(tmp = ft_itoa(g_program_return)))
			cleanup(EXIT);
		ft_memset(g_s_program_return, 0, 100);
		ft_strncpy(g_s_program_return, tmp, -1);
		free(tmp);
		ret.value = g_s_program_return;
		ret.value_len = ft_strlen(ret.value);
		return (ret);
	}
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s) && g_line->env_var[j].value)
			break ;
		j++;
	}
	return (g_line->env_var[j]);
}

t_evar	*find_env_p(char *s)
{
	size_t	j;

	j = 0;
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s))
			return (&g_line->env_var[j]);
		j++;
	}
	return (NULL);
}

t_evar	find_env(char *s)
{
	size_t	j;
	t_evar	ret;
	char	*tmp;

	j = 0;
	if (!CMP(s, "?"))
	{
		ret.name = s;
		ret.name_len = 1;
		if (!(tmp = ft_itoa(g_program_return)))
			cleanup(EXIT);
		ft_memset(g_s_program_return, 0, 100);
		ft_strncpy(g_s_program_return, tmp, -1);
		free(tmp);
		ret.value = g_s_program_return;
		ret.value_len = ft_strlen(ret.value);
		return (ret);
	}
	while (g_line->env_var[j].name)
	{
		if (!ft_strcmp(g_line->env_var[j].name, s))
			break ;
		j++;
	}
	return (g_line->env_var[j]);
}

t_evar	find_env_in_line(size_t *ref)
{
	t_evar	ret;
	size_t	i;
	size_t	j;
	char	env_space[ENV_NAME_LEN_MAX];

	(*ref)++;
	i = *ref;
	j = 0;
	while (g_line->rd.buf[i] && g_line->rd.msk[i] == ENVVAR &&
	i - *ref < ENV_NAME_LEN_MAX)
		env_space[j++] = g_line->rd.buf[i++];
	env_space[j] = '\0';
	ret = find_env(env_space);
	if (!ret.name)
	{
		ret.name_len = ft_strlen(env_space);
		ret.value_len = 0;
	}
	*ref = --i;
	return (ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int		format_string()
{
	char	*tmp;

	g_bash_errno = 0;
	if ((tmp = ft_strchr(g_line->rd.buf, '\n')) != NULL)
		*tmp = '\0';
	g_line->rd_len = ft_strlen(g_line->rd.buf);
	if (!(MALLOC(&(g_line->rd.msk), g_line->rd_len + 1)))
		cleanup(EXIT);
	set_mask();
	if (g_bash_errno || !initial_error_check())
		return (eerf(g_line->rd.msk) * 0 + g_bash_errno);
	split_wmask(&g_line->rd, &g_line->it, ';');
	free_buf_and_mask(g_line->rd);
	exec(NULL, 0, -1);
	free_and_set_to_null(&g_line->it);
	return (0);
}

void	reset_prompt(int jmp)
{
	if (g_sig == 0)
		OPRINTS("\b\b  \b\b");
	jmp ? OPRINTS("\n") : 0;
	if (g_sig == 0)
		init_read();
}

void	exit_the_shell(void)
{
	// printf("salut\n");
	OPRINTS("exit\n");
	// char *tty_name = ttyname(STDIN_FILENO);
	// int fd = open(tty_name, O_WRONLY);
	// write(fd, "exit\n", 5);
	cleanup(RETURN);
	exit(0);
}

void	ctrl_d(int *bk)
{
	reset_stdin(bk);
	OPRINTS("\e[2K");
	OPRINTS("\r");
}

void	handle_signal_quit(int sig)
{
	(void)sig;
	OPRINTS("\e[2K");
	OPRINTS("\r");
	reset_prompt(0);
}

void	handle_signal_int(int sig)
{
	(void)sig;
	reset_prompt(1);
	g_program_return = 1;
}

void	handle_error(int ex)
{
	exit(ex);
}

void	print_color(int color)
{
	if (color == 0)
		BPRINTS(ESC_RED_B);
	else if (color == 1)
		BPRINTS(ESC_ORANGE_B);
	else if (color == 2)
		BPRINTS(ESC_YELLOW_B);
	else if (color == 3)
		BPRINTS(ESC_LIME_B);
	else if (color == 4)
		BPRINTS(ESC_GREEN_B);
	else if (color == 5)
		BPRINTS(ESC_TURQUOISE_B);
	else if (color == 6)
		BPRINTS(ESC_CYAN_B);
	else if (color == 7)
		BPRINTS(ESC_BLUE_B);
	else if (color == 8)
		BPRINTS(ESC_PINK_B);
	else if (color == 9)
		BPRINTS(ESC_PURPLE_B);
	else if (color == 10)
		BPRINTS(ESC_MAGENTA_B);
}

void	skittles(char *s)
{
	static int	color;
	int			i;

	i = -1;
	while (s[++i])
	{
		print_color(color);
		// bwrite(STDOUT_FILENO, &(s[i]), 1);
		BPRINTC(s[i]);
		color++;
		color %= 11;
	}
}

void	init_read()
{
	t_evar	home;
	t_evar	user;
	t_evar return_status;

	home = find_env("HOME");
	return_status = find_env("?");
	g_pwd.name = "PWD";
	g_pwd.value = getcwd(NULL, 0);
	g_pwd.value_len = ft_strlen(g_pwd.value);
	user = find_env("USER");
	ft_memset(g_bash_error, '\0', ARG_MAX + 2);
	g_bash_errno = 0;
	g_bash_commandid = 0;
	skittles(user.value);
	skittles("@minishell");
	BPRINTS(ESC_RESET ":") * 0 + 1 && BPRINTS(ESC_BLUE_B);
	if (ft_strstr(g_pwd.value, home.name ? home.value : NULL) == g_pwd.value)
		BPRINTS("~") * 0 + 1 && BPRINT(g_pwd.value + home.value_len);
	else
		BPRINT(g_pwd.value);
	BPRINTS(ESC_RESET "$ ");
	bflush(STDOUT_FILENO);
	free(g_pwd.value);
	free(g_line->rd.buf);
	g_line->rd.buf = NULL;
}

void	continue_init_env()
{
	int		i;

	i = -1;
	while (g_line->envp[++i])
	{
		g_line->env_var[i].name_len = ft_strchr(g_line->envp[i], '=') - g_line->envp[i];
		if (!(MALLOC(&(g_line->env_var[i].name), g_line->env_var[i].name_len + 1)))
			cleanup(EXIT);
		g_line->env_var[i].name[g_line->env_var[i].name_len] = '\0';
		ft_strncpy(g_line->env_var[i].name,
		g_line->envp[i], g_line->env_var[i].name_len);
		if (!(g_line->env_var[i].value =
		ft_strdup(ft_strchr(g_line->envp[i], '=') + 1)))
			cleanup(EXIT);
		g_line->env_var[i].value_len = ft_strlen(g_line->env_var[i].value);
	}
	set_name_only(0, NULL, 0, NULL);
}

void	init_env()
{
	int		i;

	if (!g_line->envp)
		cleanup(EXIT);
	i = 0;
	while (g_line->envp[i])
		i++;
	if (!(MALLOC(&(g_line->env_var), sizeof(*(g_line->env_var)) * (i + 1))))
		cleanup(EXIT);
	i = -1;
	while (g_line->envp[++i])
	{
		g_line->env_var[i].name = NULL;
		g_line->env_var[i].value = NULL;
	}
	g_line->env_var[i].name = NULL;
	g_line->env_var[i].name_len = -1;
	g_line->env_var[i].value_len = -1;
	if (!(MALLOC(&(g_line->env_var[i].value), 1)))
		cleanup(EXIT);
	g_line->env_var[i].value[0] = '\0';
	continue_init_env();
}

void	init_envp(char **envp)
{
	int		i;
	int		len;

	if (!envp)
		cleanup(EXIT);
	i = 0;
	while (envp[i])
		i++;
	if (!(MALLOC(&(g_line->envp), 8 * i + 8)))
		cleanup(EXIT);
	g_line->envp[i] = NULL;
	i = -1;
	while (envp[++i])
	{
		len = ft_strlen(envp[i]);
		g_line->envp[i] = NULL;
		if (!(MALLOC(&(g_line->envp[i]), len + 1)))
			cleanup(EXIT);
		ft_strncpy(g_line->envp[i], envp[i], len + 1);
	}
}

void	init_buf()
{
	if (!(MALLOC(&(g_bash_error), ARG_MAX + 2)))
		exit(1);
}

void	init_line()
{
	g_line->rd.buf = NULL;
	g_line->rd.msk = NULL;
	g_line->env_var = NULL;
	g_line->env.buf = NULL;
	g_line->env.msk = NULL;
	g_line->scol = NULL;
	g_line->pipe = NULL;
	g_line->redir = NULL;
	g_line->envp = NULL;
	g_line->fd = NULL;
}

void	init_globals(t_line *ref)
{
	g_line = ref;
	g_bw.buf = NULL;
	g_bw.buf_i = NULL;
	g_sig = 0;
	g_builtin_errno = 0;
	g_bash_errno = 0;
	g_bash_commandid = 0;
	g_bash_error = NULL;
}

void	init(t_line *ref, char **envp)
{
	static char	*names[9] = { "minishell", "echo", "cd", "pwd", "export",
	"unset", "env", "exit", NULL }, *b_errors[N_B_ERROR];

	b_errors[EB_UNSET_EXPORT_NVI] = "not a valid identifier";
	b_errors[EB_CD_HNT] = "HOME not set";
	b_errors[EB_CD_ONT] = "OLDPWD not set";
	b_errors[EB_CD_EXIT_TMA] = "too many arguments";
	b_errors[EB_EXIT_NAR] = "numeric argument required";
	b_errors[N_B_ERROR - 1] = NULL;
	g_bash_command = names;
	g_builtin_error = b_errors;
	init_globals(ref);
	init_line();
	init_buf();
	init_envp(envp);
	init_env();
}

void	capture_signals(int ac, char **av)
{
	(void)ac;
	(void)av;
	signal(SIGINT, handle_signal_int);
	signal(SIGQUIT, handle_signal_quit);
}

int		main(int ac, char **av, char **envp)
{
	t_line	line;
	int		stdin_bak;

	capture_signals(ac, av);
	init(&line, envp);
	backup_stdin(&stdin_bak);
	while (1)
	{
		init_read();
		line.rd_ret = get_next_line(&line.rd.buf);
		if (line.rd_ret < 0)
			handle_error(1);
		else if (!ft_strchr(line.rd.buf, '\n') && line.rd_ret)
			ctrl_d(&stdin_bak);
		else if (line.rd_ret == 0)
			exit_the_shell();
		else
		{
			if (format_string())
				bash_error();
			free_and_set_to_null(&g_line->rd.buf);
		}
	}
}
