/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_darr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:09:28 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/10/31 14:21:06 by cdomet-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>

void	free_darr(char **darr)
{
	size_t	i;

	i = 0;
	if (!darr)
		return ;
	while (darr[i])
	{
		free(darr[i]);
		i++;
	}
	free(darr);
	darr = NULL;
	return ;
}
