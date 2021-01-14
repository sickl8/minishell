/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 17:50:05 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*back(char *dst, char *src, size_t len)
{
	while (len > 0)
	{
		dst[len - 1] = src[len - 1];
		len--;
	}
	return ((void*)dst);
}

void		*ft_memmove(void *dst, const void *src, size_t len)
{
	if ((size_t)dst > (size_t)src && (size_t)dst < (size_t)src + len)
		return (back((char*)dst, (char*)src, len));
	return (ft_memcpy(dst, src, len));
}
