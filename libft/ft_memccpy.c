/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 16:03:55 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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
