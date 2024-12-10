/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:14:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/23 19:39:06 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*empty_check(char *string)
{
	if (!string)
		return ("(null)");
	return (string);
}

static int	print_main(char *string, va_list args, int i, int fd)
{
	int	count;

	count = 0;
	if (string[i] == 'c')
		count = ft_putchar_fd(va_arg(args, int), fd);
	else if (string[i] == 's')
		count = ft_putstr_fd(empty_check(va_arg(args, char *)), fd);
	else if (string[i] == 'p')
		count = ft_putnbr_hex_fd((size_t)va_arg(args, void *), fd, 3);
	else if (string[i] == 'd' || string[i] == 'i')
		count = ft_putnbr_fd(va_arg(args, int), fd);
	else if (string[i] == 'u')
		count = ft_putnbr_fd(va_arg(args, unsigned int), fd);
	else if (string[i] == 'x')
		count = ft_putnbr_hex_fd(va_arg(args, unsigned int), fd, 1);
	else if (string[i] == 'X')
		count = ft_putnbr_hex_fd(va_arg(args, unsigned int), fd, 2);
	else if (string[i] == '%')
		count = ft_putchar_fd('%', fd);
	return (count);
}

int	ft_printf(const char *string, ...)
{
	int			count;
	int			i;
	va_list		args;

	if (!string)
		return (0);
	i = 0;
	count = 0;
	va_start (args, string);
	while (string[i])
	{
		if (string[i] == '%' && ft_strchr("cspdiuxX%", string[i + 1]))
			count += print_main((char *)string, args, 1 + i++, 1);
		else
			count += ft_putchar_fd(string[i], 1);
		i++;
	}
	va_end(args);
	return (count);
}

int	ft_dprintf(int fd, const char *string, ...)
{
	int			count;
	int			i;
	va_list		args;

	if (!string)
		return (0);
	i = 0;
	count = 0;
	va_start (args, string);
	while (string[i])
	{
		if (string[i] == '%' && ft_strchr("cspdiuxX%", string[i + 1]))
			count += print_main((char *)string, args, 1 + i++, fd);
		else
			count += ft_putchar_fd(string[i], fd);
		i++;
	}
	va_end(args);
	return (count);
}
