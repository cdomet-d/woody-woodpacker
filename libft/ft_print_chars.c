/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_chars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:48:57 by cdomet-d          #+#    #+#             */
/*   Updated: 2023/12/26 12:48:10 by cdomet-d         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	printc(va_list arg_list)
{
	char	arg;

	arg = va_arg(arg_list, int);
	return (ft_putchar_fd(arg, 1));
}

ssize_t	printstr(va_list arg_list)
{
	char	*arg;

	arg = va_arg(arg_list, char *);
	if (!arg)
		return (ft_putstr_fd("(null)", 1));
	if (!arg[0])
		return (0);
	return (ft_putstr_fd(arg, 1));
}
