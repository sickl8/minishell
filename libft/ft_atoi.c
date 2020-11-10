/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 19:15:02 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/18 16:28:57 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if ((unsigned long)__LONG_MAX__ < (unsigned long)nbr * 10 + str[i] - 48
				&& sign == 1)
			return (-1);
		else if ((unsigned long)__LONG_MAX__ < (unsigned long)nbr * 10 + str[i]
				- 48 && sign == -1)
			return (0);
		nbr = nbr * 10 + str[i++] - 48;
	}
	return (nbr * sign);
}
