/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nums.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:35:25 by cdomet-d          #+#    #+#             */
/*   Updated: 2023/12/26 12:48:54 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	printd(va_list arg_list, char flag)
{
	int	arg;
	int	counter;
	int	*count;

	counter = 0;
	count = &counter;
	arg = va_arg(arg_list, int);
	if (arg < 0 && flag == 'u')
		return (ft_u_putnbr_base(arg, DEC, count));
	else
		return (ft_putnbr_base(arg, DEC, count));
}

ssize_t	printx(va_list arg_list, char flag)
{
	int	arg;
	int	counter;
	int	*count;

	counter = 0;
	count = &counter;
	arg = va_arg(arg_list, int);
	if (arg < 0 && flag == 'x')
		return (ft_u_putnbr_base(arg, LOWER_HEXA, count));
	else if (arg >= 0 && flag == 'x')
		return (ft_putnbr_base(arg, LOWER_HEXA, count));
	else if (arg < 0 && flag == 'X')
		return (ft_u_putnbr_base(arg, UPPER_HEXA, count));
	else
		return (ft_putnbr_base(arg, UPPER_HEXA, count));
}

ssize_t	printp(va_list arg_list)
{
	size_t		arg;
	int			counter;
	int			*count;

	counter = 0;
	count = &counter;
	arg = va_arg(arg_list, size_t);
	if (!arg)
		return (ft_putstr_fd("(nil)", 1));
	return (ft_p_putnbr(arg, count));
}

ssize_t	printpercent(void)
{
	return (ft_putchar_fd('%', 1));
}
