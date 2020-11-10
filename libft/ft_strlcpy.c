/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:37:28 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/18 23:51:20 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t j;
	size_t k;

	i = 0;
	while (dst[i] != '\0')
		i++;
	j = 0;
	k = i;
	i = j;
	while (src[j] != '\0' && j < size && i + 1 < size)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	while (src[j] != '\0')
		j++;
	if (size != 0)
		dst[i] = '\0';
	return (j);
}
