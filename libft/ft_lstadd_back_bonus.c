/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 00:23:40 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/21 22:10:55 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *tmp;

	if (!(*alst))
	{
		*alst = new;
		return ;
	}
	if (alst && new)
	{
		tmp = *alst;
		while ((*alst)->next)
			*alst = (*alst)->next;
	}
	else
		return ;
	new->next = (*alst)->next;
	(*alst)->next = new;
	*alst = tmp;
}
