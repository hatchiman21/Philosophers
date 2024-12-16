/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/16 05:14:43 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	kill_the_rest(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->philos_count)
		philos->process[i++]->is_killed = 1;
	i = 0;
	while (i < philos->philos_count)
	{
		pthread_join(philos->process[i]->thread, NULL);
		free(philos->process[i++]);
	}
	i = 0;
	while (i < philos->philos_count)
		pthread_mutex_destroy(&philos->fork[i++]);
	pthread_mutex_destroy(&philos->log_mutex);
	free(philos->process);
	free(philos->fork);
	free(philos);
}

void	make_threads(t_philo *philo_data)
{
	int	i;

	pthread_mutex_init(&philo_data->log_mutex, NULL);
	i = 0;
	while (i < philo_data->philos_count)
		pthread_mutex_init(&philo_data->fork[i++], NULL);
	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, 3);
		philo_data->process[i]->philo_num = i;
		philo_data->process[i]->philo_data = philo_data;
		philo_data->process[i]->is_dead = 0;
		if (i % 3 == 0)
			philo_data->process[i]->state = EATING;
		else if (i % 3 == 1)
			philo_data->process[i]->state = THINKING;
		else
			philo_data->process[i]->state = SLEEPING;
		pthread_create(&philo_data->process[i]->thread,
			NULL, &philo_life, philo_data->process[i]);
		i++;
	}
}

t_philo	*assign_philo(char *argv[])
{
	t_philo			*philo_data;
	unsigned long	size;

	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		philo_error_handling(NULL, 2);
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	size = sizeof(t_philo_process *) * philo_data->philos_count;
	philo_data->process = malloc(size);
	if (!philo_data->process)
		philo_error_handling(philo_data, 3);
	philo_data->death = 0;
	size = sizeof(pthread_mutex_t) * philo_data->philos_count;
	philo_data->fork = malloc(size);
	if (!philo_data->fork)
		philo_error_handling(philo_data, 4);
	return (philo_data);
}

int	main(int argc, char *argv[])
{
	t_philo	*philo_data;

	check_input(argv, argc);
	philo_data = assign_philo(argv);
	make_threads(philo_data);
	while (philo_data->death == 0)
		usleep(10);
	kill_the_rest(philo_data);
	return (0);
}
