/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:57:00 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/23 19:16:09 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_hex_fd(unsigned long n, int fd, int j)
{
	int	i;

	i = 1;
	if (j == 3 && n != 0)
	{
		write (1, "0x", 2);
		i += 2;
	}
	else if (j == 3 && n == 0)
		return (ft_putstr_fd("(nil)", fd));
	if (n > 15)
	{
		if (j == 2)
			i += ft_putnbr_hex_fd(n / 16, fd, 2);
		else
			i += ft_putnbr_hex_fd(n / 16, fd, 1);
	}
	if ((n % 16) < 10)
		ft_putchar_fd((n % 16) + '0', fd);
	else if (j == 2)
		ft_putchar_fd((n % 16) - 10 + 'A', fd);
	else
		ft_putchar_fd((n % 16) - 10 + 'a', fd);
	return (i);
}
