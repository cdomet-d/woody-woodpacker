/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:14:01 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/05/02 15:40:34 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static char	leftover[BUFFER_SIZE + 1] = {0};
	char		*buff;
	int			check;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE < 1)
		return (NULL);
	check = 1;
	buff = NULL;
	if (leftover[0])
	{
		buff = ft_substr(leftover, 0, BUFFER_SIZE);
		if (!buff)
			return (NULL);
		ft_bzero(leftover, BUFFER_SIZE + 1);
	}
	while (check > 0 && !ft_strchr(buff, '\n'))
	{
		buff = ft_read(buff, &check, fd);
		if (!buff)
			return (NULL);
	}
	if (ft_len(buff, '\n') < 0 || ft_len(buff, '\n') == ft_len(buff, '\0'))
		return (buff);
	else
		return (ft_split_buff(buff, leftover));
}

char	*ft_read(char *buff, int *check, int fd)
{
	char	*current_read;
	char	*temp;

	current_read = ft_calloc(sizeof (char), BUFFER_SIZE + 1);
	if (!current_read)
		return (ft_free(buff, NULL));
	*check = read(fd, current_read, BUFFER_SIZE);
	if (*check == -1)
		return (ft_free(current_read, buff));
	temp = ft_join_buff(buff, current_read);
	if (!temp)
		return (ft_free(current_read, buff));
	if (buff)
		free(buff);
	buff = ft_substr(temp, 0, ft_len(temp, '\0'));
	ft_free(current_read, temp);
	if (!buff)
		return (NULL);
	return (buff);
}

char	*ft_split_buff(char *buff, char *leftover)
{
	char	*line;
	size_t	i;
	size_t	j;

	j = 0;
	i = ft_len(buff, '\n');
	line = ft_substr(buff, 0, i);
	if (!line)
		return (ft_free(buff, NULL));
	while (buff[i])
		leftover[j++] = buff[i++];
	ft_free(buff, NULL);
	return (line);
}
