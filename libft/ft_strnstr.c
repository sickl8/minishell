/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:38:58 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/14 12:27:50 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t i;
	size_t s;

	i = ft_strlen(needle) - 1;
	s = 0;
	if (*needle == '\0')
		return ((char*)haystack);
	while (*haystack != '\0' && len > 0)
	{
		if (*needle == *haystack)
		{
			while (needle[s] == haystack[s] && haystack[s] && len - s > 0)
			{
				if (s == i)
					return ((char*)haystack);
				s++;
			}
			s = 0;
		}
		haystack++;
		len--;
	}
	return (0);
}
