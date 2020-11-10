/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 01:28:15 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/14 12:27:45 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*rtn;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	rtn = ft_strdup(s);
	if (rtn == NULL)
		return (NULL);
	while (rtn[i])
	{
		rtn[i] = f(i, rtn[i]);
		i++;
	}
	return (rtn);
}
