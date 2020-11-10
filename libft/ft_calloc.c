/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 23:06:14 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/12 23:15:20 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	char			*tmp;
	size_t			i;

	i = 0;
	ptr = malloc(size * count);
	if (ptr == NULL)
		return (NULL);
	tmp = ptr;
	while (i < size * count)
	{
		*tmp = 0;
		i++;
		tmp++;
	}
	return (ptr);
}
