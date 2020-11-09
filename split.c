/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msplit_whitespace.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 17:42:15 by isaadi            #+#    #+#             */
/*   Updated: 2020/11/04 12:13:35 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#define WHTSPC 'W'
#define NONLIT 'N'
#define ENVVAR 'V'
#define SEMILIT 'S'
#define LITERAL 'L'
#define BASHSYN 'B'
#define H 1
// #define H printf("here: %d\n", __LINE__)

#define T(x, y, z) (x ? y : z)

typedef struct	s_line
{
	char	*rd_buf;
	char	*mask;
	char	*msk;
	ssize_t	rd_ret;
	char	**words;
}				t_line;
int		ws(char c);

void	*chetaba(char **ret, size_t i)
{
	i--;
	while (i > 0)
		free(ret[i--]);
	free(ret);
	return (NULL);
}

void	*split(char **ret, t_line line, size_t wc)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	len;

	i = 0;
	k = 0;
	while (line.rd_buf[i] && 1 + 0 * H)
	{
		H;
		if (line.msk[i] == 'S')
		{
			len = 0;H;
			j = -1;H;
			while (line.rd_buf[i + ++j] && line.mask[i + j] != WHTSPC)
				len += (line.mask[i + j] == SEMILIT || line.mask[i + j] == LITERAL || line.mask[i + j] == NONLIT);
			if (!(ret[k] = malloc(sizeof(ret[k][0]) * (len + 1))))
				return (chetaba(ret, k));
			H;
			ret[k][len] = '\0';H;
			j = -1;H;
			len = 0;H;
			while (line.rd_buf[i + ++j] && line.mask[i + j] != WHTSPC)
				if (line.mask[i + j] == SEMILIT || line.mask[i + j] == LITERAL || line.mask[i + j] == NONLIT)
					ret[k][len++] = line.rd_buf[i + j] + 0 * H;
			k++;
			i += j;
		}
		else
			i++;
	}
	return (ret);
}

int		ws(char c)
{
	if (c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == ' ' || c == 'v'
	|| c == '\0')
		return (1);
	return (0);
}

size_t	word_count(char *m, char *n)
{
	size_t	wc;
	size_t	i;
	char	c;

	wc = (m[0] != WHTSPC);
	if (m[0] != WHTSPC)
		n[0] = 'S';
	i = -1;
	while (m[++i])
	{
		if (m[i] && m[i + 1] && m[i] == WHTSPC && m[i + 1] != WHTSPC)
		{
			n[i + 1] = 'S';
			wc++;
		}
	}
	return (wc);
}

char	**ft_msplit_whitespace(t_line line)
{
	char	**ret;
	char	*s;
	char	*m;
	size_t	wc;

	s = line.rd_buf;
	m = line.mask;
	wc = word_count(line.mask, line.msk);
	if (!(ret = malloc(sizeof(*ret) * (wc + 1))))
		return (NULL);
	ret[wc] = NULL;
	ret = split(ret, line, wc);
	return (ret);
}

void	whtspc_or_nonlit(t_line line, size_t *ref)
{
	size_t	i;
	char	c;

	i = *ref;
	c = line.rd_buf[i];
	if (c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == ' ' || c == 'v'
	|| c == '\0')
		line.mask[i] = WHTSPC;
	else
		line.mask[i] = NONLIT;
	*ref = i;
}

int		is_env_comp(char c)
{
	if ((c <= 'Z' && c >= 'A') || (c <= '9' && c >= '0') || c == '_')
		return (1);
	return (0);
}

void	env_var(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i++] = '$';
	while (line.rd_buf[i] && is_env_comp(line.rd_buf[i]))
		line.mask[i++] = ENVVAR;
	i--;
	*ref = i;
}

void	bash_syntax(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	if (line.rd_buf[i] == '|' || line.rd_buf[i] == '>' || line.rd_buf[i] == '<' || line.rd_buf[i] == ';')
	{
		line.mask[i++] = BASHSYN;
		if (line.rd_buf[i] == '>')
			line.mask[i] = BASHSYN;
	}
	*ref = i;
}

void	backslach(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i++] = '\\';
	if (line.rd_buf[i])
		line.mask[i] = LITERAL;
	*ref = i;
}

void	push_double_quote(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i++] = '"';
	
	while (line.rd_buf[i] && line.rd_buf[i] != '"')
	{
		if (line.rd_buf[i] == '\\')
		{
			line.mask[i++] = '\\';
			line.mask[i++] = LITERAL;
		}
		else
			line.mask[i++] = SEMILIT;
	}
	if (line.rd_buf[i] == '"')
		line.mask[i] = '"';
	else
		i--;
	*ref = i;
}

void	push_single_quote(t_line line, size_t *ref)
{
	size_t	i;

	i = *ref;
	line.mask[i++] = '\'';
	while (line.rd_buf[i] && line.rd_buf[i] != '\'')
	{
		line.mask[i] = LITERAL;
		i++;
	}
	if (line.rd_buf[i] == '\'')
		line.mask[i] = '\'';
	else
		i--;
	*ref = i;
}

void	set_mask(t_line line)
{
	size_t	i;

	i = -1;
	while (line.rd_buf[++i])
	{
		if (line.rd_buf[i] == '\'')
			push_single_quote(line, &i);
		else if (line.rd_buf[i] == '"')
			push_double_quote(line, &i);
		else if (line.rd_buf[i] == '\\')
			backslash(line, &i);
		else if (line.rd_buf[i] == '|' || line.rd_buf[i] == '>' ||
		line.rd_buf[i] == '<' || line.rd_buf[i] == ';')
			bash_syntax(line, &i);
		else if (line.rd_buf[i] == '$')
			env_var(line, &i);
		else
			whtspc_or_nonlit(line, &i);
	}
	line.mask[i] = '\0';
}

// int main(int ac, char **av)
// {
// 	char **ret;
// 	char *m;
// 	t_line line;
// 	if (ac > 1)
// 	{
// 		line.rd_buf = av[1];
// 		line.rd_ret = ft_strlen(line.rd_buf);
// 		line.mask = malloc(sizeof(char) * (line.rd_ret + 2));
// 		line.msk = malloc(sizeof(char) * (line.rd_ret + 3));
// 		ft_memset(line.mask, '\0', line.rd_ret + 2);
// 		ft_memset(line.msk, ' ', line.rd_ret + 2);
// 		ft_memset(line.msk + line.rd_ret + 1, '\0', 1);
// 		set_mask(line);
// 		// printf("stl = %zu\n", line.rd_ret);
// 		// printf("str = %p\n", line.rd_buf);
// 		// printf("msk = %p\n", line.mask);
// 		printf("str = %s\n", line.rd_buf);
// 		printf("msk = %s\n", line.mask);
// 		ret = ft_split_whitespace(line);
// 		// printf("here?\n");
// 		if (ret)
// 			for (int i = 0; ret[i] != NULL; i++)
// 				printf("word[%d] = \"%s\"\n", i, ret[i]);
// 		/*******************************************************************/


// 						//SEG IN PRINTF?


// 		/*******************************************************************/
// 	}
// }