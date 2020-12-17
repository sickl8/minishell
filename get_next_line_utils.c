/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 17:11:02 by isaadi            #+#    #+#             */
/*   Updated: 2020/12/17 20:38:39 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_slen(const char *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i])
		i++;
	return (i);
}

char	*fs(const char *s, int c)
{
	char a;
	char *z;

	if (!s)
		return (NULL);
	z = (char*)s;
	a = (char)c;
	while (*z)
	{
		if (*z == a)
			return (z);
		z++;
	}
	if (a == '\0')
		return (z);
	return (0);
}

char	*ft_sdup(const char *s1)
{
	int		i;
	char	*rtn;

	i = 0;
	if (!s1)
		return ((char*)s1);
	while (s1[i])
		i++;
	rtn = malloc(i + 1);
	if (rtn == NULL)
		return (NULL);
	while (i >= 0)
	{
		rtn[i] = s1[i];
		i--;
	}
	return (rtn);
}

char	*ft_subs(char const *s, unsigned int start, size_t len)
{
	char	*rtn;
	size_t	i;

	if (!s)
		return (NULL);
	if (len > ft_slen((char*)s))
		len = ft_slen((char*)s);
	if (start > ft_slen((char*)s))
		start = ft_slen((char*)s);
	i = 0;
	rtn = (char*)malloc(sizeof(char) * len + 1);
	if (rtn == NULL)
		return (NULL);
	while (i < len)
	{
		rtn[i] = s[start];
		start++;
		i++;
	}
	rtn[i] = '\0';
	return (rtn);
}

char	*ft_sjoin(char const *s1, char const *s2)
{
	char	*tmp;
	size_t	i;
	size_t	j;
	size_t	k;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = -1;
	j = ft_slen(s1);
	k = ft_slen(s2);
	tmp = (char*)malloc(sizeof(char) * (j + k + 1));
	if (tmp == NULL || (char*)s2 == NULL)
		return (NULL);
	while (++i < j)
		tmp[i] = s1[i];
	i = -1;
	while (++i <= k)
		tmp[i + j] = s2[i];
	return (tmp);
}
