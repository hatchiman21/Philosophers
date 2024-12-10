/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:37:27 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/23 19:15:48 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(long n, int fd)
{
	long int	m;
	int			i;
	char		r;

	m = n;
	i = 1;
	if (m < 0)
	{
		write (fd, "-", 1);
		m *= -1;
		i++;
	}
	if (m > 9)
		i += ft_putnbr_fd(m / 10, fd);
	r = (m % 10) + '0';
	write (fd, &r, 1);
	return (i);
}
