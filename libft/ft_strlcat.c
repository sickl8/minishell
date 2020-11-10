/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:34:58 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/11 21:49:28 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t j;
	size_t k;

	i = 0;
	while (dst[i] != '\0')
		i++;
	j = 0;
	k = i;
	while (src[j] != '\0' && j < size && i < size - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	while (src[j] != '\0')
		j++;
	dst[i] = '\0';
	if (size >= k)
		return (k + j);
	else
		return (j + size);
}
