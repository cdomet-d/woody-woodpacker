/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:35:46 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/03/07 14:47:13 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*ft_callocword(char const *s, char sep, size_t current_word)
{
	size_t		len;
	size_t		start_current_word;
	char		*word;

	len = 0;
	start_current_word = current_word;
	while (s[current_word] && s[current_word] != sep)
	{
		current_word++;
		len++;
	}
	word = ft_calloc(len + 1, sizeof (char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + start_current_word, len + 1);
	return (word);
}

static size_t	ft_countwords(const char *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c && s[i - 1] != c)
			count++;
		i++;
	}
	if (s[i - 1] == c)
		count--;
	return (count + 1);
}

static void	*freeall(char **arr)
{
	size_t	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t		i;
	size_t		nb_words;
	size_t		find_next_word;
	char		**arr;

	i = 0;
	if (!s || !s[i] || !ft_countwords(s, c))
		return (NULL);
	nb_words = ft_countwords(s, c);
	arr = ft_calloc(nb_words + 1, sizeof (char *));
	if (!arr)
		return (arr);
	arr[nb_words] = NULL;
	find_next_word = 0;
	while (i < nb_words)
	{
		while (s[find_next_word] && s[find_next_word] == c)
			find_next_word++;
		arr[i] = ft_callocword(s, c, find_next_word);
		if (!arr[i])
			return (freeall(arr));
		find_next_word += ft_strlen(arr[i]);
		i++;
	}
	return (arr);
}
