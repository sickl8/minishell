/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 23:24:27 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/12 23:16:04 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*rtn;

	i = 0;
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
