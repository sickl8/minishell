/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 19:15:02 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/16 17:05:54 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define LM __LONG_MAX__

typedef unsigned long	t_ul;

int		ft_atoi(const char *str)
{
	long i;
	long nbr;
	long sign;

	sign = 1;
	nbr = 0;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\f'
			|| str[i] == '\n' || str[i] == '\r')
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign = (str[i++] == '-' ? -1 : 1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((t_ul)LM < (t_ul)nbr * 10 + str[i] - 48 && sign == 1)
			return ((sign == -1) * sign);
		nbr = nbr * 10 + str[i++] - 48;
	}
	return (nbr * sign);
}
