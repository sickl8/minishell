/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_typedef.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 13:33:39 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/15 17:11:39 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_TYPEDEF_H
# define HEADER_TYPEDEF_H

# include <stdlib.h>
# include <_ssize_t.h>

typedef struct	s_rdr
{
	int		type;
	char	*file_name;
}				t_rdr;

typedef struct	s_cmd
{
	int				fd_read;
	int				fd_write;
	char			**cpa;
	char			*find;
	char			*path2exec;
	char			**args;
	t_rdr			*redir;
	struct s_cmd	*next;
}				t_cmd;

typedef struct	s_evar
{
	char	*name;
	char	*value;
	size_t	name_len;
	size_t	value_len;
	int		name_only;
}				t_evar;

typedef struct	s_bm
{
	char	*buf;
	char	*msk;
	size_t	cnt;
}				t_bm;

typedef struct	s_line
{
	char	**envp;
	t_evar	*env_var;
	t_bm	rd;
	t_bm	*it;
	t_bm	env;
	t_bm	*scol;
	t_bm	**pipe;
	t_bm	***redir;
	long	rd_ret;
	size_t	rd_len;
	int		*fd;
}				t_line;

typedef struct	s_bw
{
	char	***buf;
	size_t	**buf_i;
}				t_bw;

typedef struct	s_fr
{
	char	*p;
	char	*rr;
	char	*rl;
	char	*rrr;
}				t_fr;

typedef	struct	s_ep
{
	char	**envp;
	int		n;
}				t_ep;

typedef struct	s_fnl
{
	t_cmd			*cmd_and_args;
	struct s_fnl	*next;
}				t_fnl;

typedef	struct	s_export
{
	int		env_len;
	int		new_env_len;
}				t_export;

#endif
