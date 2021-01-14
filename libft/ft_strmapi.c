/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 01:28:15 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
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
