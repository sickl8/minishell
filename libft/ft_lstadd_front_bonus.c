/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 00:23:53 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/21 22:15:52 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **alst, t_list *new)
{
	if (!(*alst))
		*alst = new;
	else if (new)
	{
		new->next = *alst;
		*alst = new;
	}
	else if (*alst && !new)
		*alst = new;
}
