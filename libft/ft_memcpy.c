/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 15:33:29 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/15 18:23:57 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*c;
	char	*a;

	c = (char*)dst;
	a = (char*)src;
	if (dst == NULL && src == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		c[i] = a[i];
		i++;
	}
	return (dst);
}
