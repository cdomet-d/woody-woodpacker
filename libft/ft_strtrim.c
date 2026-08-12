/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:36:50 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/10/25 18:08:44 by cdomet-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t		i;
	size_t		end;
	ssize_t		j;

	i = 0;
	if (!s1 || !set || !s1[i])
		return (NULL);
	while (s1[i] && ft_strchr(set, (int)s1[i]))
		i++;
	j = ft_strlen(s1);
	while (j >= 0 && ft_strchr(set, (int)s1[j]))
		j--;
	end = j - i + 1;
	return (ft_substr(s1, i, end));
}
