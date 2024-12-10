/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:36:00 by aatieh            #+#    #+#             */
/*   Updated: 2024/11/03 19:59:36 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (0);
	while (i < n - 1)
	{
		if (*(unsigned char *)(s1 + i) != *(unsigned char *)(s2 + i))
			break ;
		i++;
	}
	if ((*(unsigned char *)(s1 + i)) - (*(unsigned char *)(s2 + i)) > 0)
		return (1);
	else if ((*(unsigned char *)(s1 + i)) - (*(unsigned char *)(s2 + i)) < 0)
		return (-1);
	return (0);
}
