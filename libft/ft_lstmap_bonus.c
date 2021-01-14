/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isaadi <isaadi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 00:25:03 by isaadi            #+#    #+#             */
/*   Updated: 2021/01/14 19:35:32 by isaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *tmp;
	t_list **ttmp;

	if (!(ttmp = (t_list**)malloc(sizeof(t_list*))))
		return (NULL);
	if (lst)
		tmp = ft_lstnew((*f)(lst->content));
	else
		return (NULL);
	if (tmp == NULL)
		return (NULL);
	*ttmp = tmp;
	while (lst->next)
	{
		tmp->next = ft_lstnew((*f)(lst->next->content));
		if (tmp->next == NULL)
		{
			if (del)
				ft_lstclear(ttmp, del);
			return (NULL);
		}
		tmp = tmp->next;
		lst = lst->next;
	}
	return (*ttmp);
}
