/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:18:18 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/02/12 14:53:01 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*arr;
	size_t	i;

	if (!s)
		return (NULL);
	arr = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!arr)
		return (arr);
	i = -1;
	while (s && s[++i])
		arr[i] = ((char *)s)[i];
	arr[i] = '\0';
	return ((char *) arr);
}
