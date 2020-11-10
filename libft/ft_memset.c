/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:41:30 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/11 21:45:23 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	ch;
	char			*cha;

	ch = (unsigned int)c;
	cha = b;
	i = 0;
	while (i < len)
	{
		cha[i] = ch;
		i++;
	}
	return (b);
}
