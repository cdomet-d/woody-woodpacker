/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:26:46 by cdomet-d          #+#    #+#             */
/*   Updated: 2023/11/23 11:56:45 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_size(long n)
{
	int		size;
	long	sign;

	size = 0;
	sign = n;
	if (n == 0)
		size = 1;
	if (n < 0)
		n = n * -1;
	while (n > 0)
	{
		n = n / 10;
		size++;
	}
	if (sign < 0)
		return (size + 1);
	else
		return (size);
}

static long	is_negative(long n)
{
	n = n * -1;
	return (n);
}

char	*ft_itoa(int n)
{
	int			size;
	int			sign;
	long		temp_nb;
	long		nb;
	char		*str;

	nb = (long)n;
	sign = nb;
	size = num_size(nb);
	if (nb < 0)
		nb = is_negative(nb);
	str = ft_calloc(size + 1, sizeof (char));
	if (!str)
		return (str);
	while (--size >= 0)
	{
		temp_nb = nb % 10;
		str[size] = (temp_nb + 48);
		nb = nb / 10;
	}
	if (sign < 0)
		str[0] = '-';
	return (str);
}
