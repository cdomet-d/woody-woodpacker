/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdomet-d <cdomet-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:08 by cdomet-d          #+#    #+#             */
/*   Updated: 2024/10/31 14:38:18 by cdomet-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	read_str(char flag, va_list arg_list)
{
	ssize_t	check;

	if (flag == 'd' || flag == 'i' || flag == 'u' )
		return (printd(arg_list, flag));
	else if (flag == 'x' || flag == 'X')
		return (printx(arg_list, flag));
	else if (flag == '%')
		return (printpercent());
	else if (flag == 's')
		return (printstr(arg_list));
	else if (flag == 'c')
		return (printc(arg_list));
	else if (flag == 'p')
		return (printp(arg_list));
	else
	{
		check = ft_putchar_fd('%', 1);
		if (check == -1)
			return (-1);
		check = ft_putchar_fd(flag, 1);
		if (check == -1)
			return (-1);
		return (check + 1);
	}
}

ssize_t	ft_parsef(const char *s, va_list arg_list,
					size_t *count, ssize_t *len)
{
	ssize_t	check;
	size_t	i;

	i = -1;
	check = 0;
	while (s[++i])
	{
		if (s[i] != '%')
		{
			check = ft_putchar_fd(s[i], 1);
			if (check == -1)
				return (-1);
		}
		else if (s[i] == '%')
		{
			if (!s[i + 1])
				return (-1);
			(*count)++;
			check = read_str(s[++i], arg_list);
			if (check == -1)
				return (-1);
			(*len) += check;
		}
	}
	return (check);
}

int		__attribute__ ((format (printf, 1, 2)))	
	ft_printf(const char *str, ...)
{
	va_list	arg_list;
	ssize_t	check;
	ssize_t	total_len;
	size_t	count_flag;

	if (!str)
		return (-1);
	total_len = 0;
	count_flag = 0;
	va_start(arg_list, str);
	check = ft_parsef(str, arg_list, &count_flag, &total_len);
	va_end(arg_list);
	if (check < 0)
		return (-1);
	return (ft_strlen(str) + total_len - (2 * count_flag));
}
