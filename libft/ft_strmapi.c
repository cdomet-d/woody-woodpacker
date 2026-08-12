/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:40:55 by cdomet-d          #+#    #+#             */
/*   Updated: 2023/11/23 14:06:20 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	len;
	char			*result;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	result = ft_calloc(len + 1, sizeof (char));
	if (!result)
		return (NULL);
	i = -1;
	while (s[++i])
		result[i] = (*f)(i, s[i]);
	return (result);
}
