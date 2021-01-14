/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 00:46:51 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*foo(char *rtn, long p, long n)
{
	int i;

	i = 0;
	while (p > 0)
	{
		rtn[i++] = (n / p) % 10 + 48;
		p /= 10;
	}
	rtn[i] = '\0';
	return (rtn);
}

char		*ft_itoa(int n)
{
	long	p;
	char	*rtn;
	long	nb;
	int		sign;
	int		m;

	m = 1;
	p = 1;
	sign = 0;
	nb = (long)n;
	if (nb < 0)
	{
		nb *= -1;
		sign = 1;
	}
	while (p * 10 <= nb)
	{
		m++;
		p *= 10;
	}
	rtn = (char*)malloc(sizeof(char) * (m + sign + 1));
	if (rtn == NULL)
		return (NULL);
	rtn[0] = '-';
	return (foo(rtn + sign, p, nb) - sign);
}
