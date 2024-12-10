/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:57:00 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/03 20:00:03 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(long m)
{
	char	r;
	int		i;

	i = 1;
	if (!i)
		i = 0;
	if (m < 0)
	{
		write (1, "-", 1);
		m *= -1;
		i++;
	}
	if (m > 9)
		i += ft_putnbr(m / 10);
	r = (m % 10) + '0';
	write (1, &r, 1);
	return (i);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		write (1, &s[i++], 1);
	return (i);
}

int	ft_putchar(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_putnbr_hex(unsigned long n, int j)
{
	int	i;

	i = 1;
	if (j == 3 && n != 0)
	{
		write (1, "0x", 2);
		i += 2;
	}
	else if (j == 3 && n == 0)
		return (ft_putstr("(nil)"));
	if (n > 15)
	{
		if (j == 2)
			i += ft_putnbr_hex(n / 16, 2);
		else
			i += ft_putnbr_hex(n / 16, 1);
	}
	if ((n % 16) < 10)
		ft_putchar((n % 16) + '0');
	else if (j == 2)
		ft_putchar((n % 16) - 10 + 'A');
	else
		ft_putchar((n % 16) - 10 + 'a');
	return (i);
}
