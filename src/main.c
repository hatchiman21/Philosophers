/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/10 22:02:52 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	phil_error_handling(t_philo	*phil, int error)
{
	if (error = 1)
		ft_dprintf(2, "wrong input format\n");
	else if (error == 2)
		ft_dprintf(2, "malloc failed\n");
	exit(error);
}

t_philo	*check_assgin(char *argv[])
{
	int		i;
	int		j;
	t_philo	*phil;

	i = 0;
	while (i++ < 4)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			phil_error_handling(NULL, 1);
	}
	phil = malloc(sizeof(t_philo));
	if (!phil)
		phil_error_handling(NULL, 2);
	phil->philo_num = ft_atoi(argv[1]);
	phil->t_to_die = ft_atoi(argv[2]);
	phil->t_to_eat = ft_atoi(argv[3]);
	phil->t_to_sleep = ft_atoi(argv[4]);
	phil->first_child = NULL;
	return (phil);
}

int	main(int argc, char *argv[])
{
	t_philo	*phil;

	if (argc != 5)
	{
		ft_dprintf(2, "wrong number of inputs\n");
		return (1);
	}
	phil = check_assgin(argv);
	return (0);
}
