/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 18:15:42 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/14 13:12:06 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	b;
	unsigned char	*d;

	d = (unsigned char*)s;
	b = (unsigned char)c;
	while (n > 0)
	{
		if ((unsigned char)*d == b)
			return ((void*)d);
		n--;
		d++;
	}
	return (0);
}
