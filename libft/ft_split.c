/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 21:58:45 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/19 22:32:42 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**trash(char ***tab, int j)
{
	while (j > 0)
		free(tab[0][--j]);
	free(tab[0]);
	return (NULL);
}

static char	**ft_insert(char **tab, char *s, char c)
{
	int i;
	int j;

	j = 0;
	while (*s)
	{
		i = 0;
		while (*s == c && s)
			s++;
		while (s[i] != c && s[i])
			i++;
		tab[j] = (char*)malloc(sizeof(char) * (i + 1));
		if (tab[j] == NULL)
			return (trash(&tab, j));
		tab[j][i] = '\0';
		*s ? ft_strlcpy(tab[j++], s, i + 1) : 0;
		s += i;
	}
	tab[j] = NULL;
	return (tab);
}

static int	ft_wc(char *str, char c)
{
	int wc;
	int i;

	i = 0;
	wc = 0;
	while (str[i])
	{
		if (str[i] != c && (i == 0 || str[i - 1] == c))
			wc++;
		i++;
	}
	return (wc);
}

char		**ft_split(char const *s, char c)
{
	char	**tab;

	if (s)
	{
		tab = (char**)malloc(sizeof(char*) * (ft_wc((char*)s, c) + 1));
		if (!tab)
			return (NULL);
	}
	else
		return (NULL);
	return (ft_insert(tab, (char*)s, c));
}
