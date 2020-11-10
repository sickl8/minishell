/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 16:03:55 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/15 18:17:37 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c,
		size_t n)
{
	size_t			i;
	unsigned char	a;
	unsigned char	*q;
	unsigned char	*w;
	void			*rtn;

	rtn = (void*)dst;
	q = (unsigned char*)dst;
	w = (unsigned char*)src;
	a = c;
	i = 0;
	while (i < n)
	{
		q[i] = w[i];
		i++;
		rtn++;
		if (i > 0)
		{
			if (w[i - 1] == a)
				return (rtn);
		}
	}
	return (0);
}
