/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 17:11:49 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/19 22:57:56 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_ctrim(char *s2, char *s3)
{
	char	*rtn;
	int		i;

	i = (unsigned long)s3 - (unsigned long)s2 + 1;
	rtn = (char*)malloc(sizeof(char) * (i + 1));
	if (rtn == NULL)
		return (NULL);
	i = 0;
	while (s2 != s3)
	{
		rtn[i] = *s2;
		i++;
		s2++;
	}
	rtn[i] = *s2;
	rtn[i + 1] = '\0';
	return (rtn);
}

static char	*f(void)
{
	char *s;

	s = (char*)malloc(sizeof(char) * 1);
	if (!s)
		return (NULL);
	*s = '\0';
	return (s);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	char	*s3;
	char	*s2;

	if (!s1 || !set)
		return (NULL);
	s2 = (char*)s1;
	s3 = (char*)s1 + ft_strlen(s1) - 1;
	i = -1;
	while (set[++i])
		if (*s2 == set[i])
		{
			s2++;
			i = -1;
		}
	i = -1;
	while (set[++i])
		if (*s3 == set[i])
		{
			s3--;
			i = -1;
		}
	return (!(*s2) ? f() : ft_ctrim(s2, s3));
}
