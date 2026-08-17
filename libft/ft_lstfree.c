/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:52:20 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/02/12 14:52:11 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"

t_list	*ft_lstfree(t_list **lst)
{
	t_list	*temp;

	if (lst && *lst)
	{
		while ((*lst)->next)
		{
			temp = (*lst)->next;
			free((*lst)->data);
			free(*lst);
			*lst = temp;
		}
		free(*lst);
	}
	*lst = NULL;
	return (NULL);
}
