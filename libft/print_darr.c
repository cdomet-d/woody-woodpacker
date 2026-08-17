/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_darr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:07:34 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/10/18 13:06:27 by cdomet-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"

void	print_darr(char **arr, bool verbose)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	printf("adress: %p\n", arr);
	if (!arr || !(*arr))
		return ;
	while (arr[i])
	{
		j = 0;
		if (verbose)
		{
			printf("%ld |", i);
			while (arr[i][j])
				printf(" %c |", arr[i][j++]);
			printf("\n");
		}
		else
			printf("%s\n", arr[i]);
		i++;
	}
}
