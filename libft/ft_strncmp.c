/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 22:26:43 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/19 00:00:36 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	n = n > ft_strlen(s1) ? ft_strlen(s2) + 1 : n;
	n = n > ft_strlen(s2) ? ft_strlen(s2) + 1 : n;
	return (ft_memcmp((const void*)s1, (const void*)s2, n));
}
