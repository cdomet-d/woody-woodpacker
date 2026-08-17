/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:56:09 by cdomet-d          #+#    #+#             */
/*   Updated: 2023/11/23 13:57:00 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	if (!dst && !size)
		return (ft_strlen(src));
	len = ft_strlen(dst);
	if (len > size)
		len = size;
	if (len < size)
	{
		i = -1;
		while (src[++i] && len + i < size - 1)
			dst[len + i] = src[i];
		dst[len + i] = '\0';
	}
	return (len + ft_strlen(src));
}
