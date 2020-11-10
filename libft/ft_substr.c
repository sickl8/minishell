/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 23:33:01 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/18 16:56:54 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*rtn;
	size_t	i;

	if (!s)
		return (NULL);
	if (len > ft_strlen((char*)s))
		len = ft_strlen((char*)s);
	if (start > ft_strlen((char*)s))
		start = ft_strlen((char*)s);
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
