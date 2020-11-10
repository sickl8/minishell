/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 00:24:05 by isaadi            #+#    #+#             */
/*   Updated: 2019/10/20 23:39:48 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *tmp;
	t_list *tmp2;

	tmp2 = *lst;
	tmp = tmp2;
	while (tmp2 && del)
	{
		tmp = tmp2;
		tmp2 = tmp2->next;
		if (tmp)
			(*del)(tmp->content);
		if (tmp)
			free(tmp);
	}
	*lst = NULL;
}
