/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 18:39:27 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/14 13:16:29 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	i = 0;
	b = (unsigned char*)s2;
	a = (unsigned char*)s1;
	if (n == 0)
		return (0);
	while (a[i] == b[i] && i + 1 < n)
		i++;
	return (a[i] - b[i]);
}
