/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:04:01 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/17 00:54:12 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tmp;
	size_t	i;
	size_t	j;
	size_t	k;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = -1;
	j = ft_strlen(s1);
	k = ft_strlen(s2);
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
