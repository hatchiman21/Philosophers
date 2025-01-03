/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/01 17:40:17 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	check_input(char *argv[], int argc)
{
	int		i;
	int		j;

	if (argc != 5)
	{
		write(2, "wrong number of inputs\n", 24);
		exit (1);
	}
	i = 0;
	while (i++ < argc - 1)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			philo_error_handling(NULL, 0, 1);
	}
}

void	kill_the_rest(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->philos_count)
	{
		pthread_join(philos->process[i]->thread, NULL);
		free(philos->process[i++]);
	}
	pthread_mutex_destroy(&philos->log_mutex);
	pthread_mutex_destroy(&philos->sim_stop_mutex);
	pthread_mutex_destroy(&philos->meal_mutex);
	free(philos->process);
	free(philos);
}

void	make_threads(t_philo *philo_data)
{
	int	i;

	i = 0;
	philo_data->start_time = get_time_in_ms() + (philo_data->philos_count * 20);
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, i, 7);
		philo_data->process[i]->philo_num = i;
		philo_data->process[i]->philo_data = philo_data;
		philo_data->process[i]->state = EATING;
		philo_data->process[i]->last_meal = philo_data->start_time;
		i++;
	}
	i = 0;
	while (i < philo_data->philos_count)
	{
		if (pthread_create(&philo_data->process[i]->thread,
				NULL, &philo_life, philo_data->process[i]) != 0)
			philo_error_handling(philo_data, i, 8);
		i++;
	}
}

t_philo	*assign_philo(char *argv[])
{
	t_philo			*philo_data;
	unsigned long	size;

	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		philo_error_handling(NULL, 0, 2);
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	size = sizeof(t_philo_process *) * philo_data->philos_count;
	philo_data->process = NULL;
	philo_data->process = malloc(size);
	if (!philo_data->process)
		philo_error_handling(philo_data, 0, 3);
	sem_init(&philo_data->sim_stop_mutex, 1, 1);
	philo_data->sim_stop = 0;
	size = sizeof(t_eating_fork) * philo_data->philos_count;
	sem_init(&philo_data->fork, 1, philo_data->philos_count);
	return (philo_data);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_philo	*philo_data;

	check_input(argv, argc);
	philo_data = assign_philo(argv);
	// if (pthread_mutex_init(&philo_data->log_mutex, NULL) != 0)
	// 	philo_error_handling(philo_data, 0, 5);
	sem_init(&philo_data->log_mutex, 1, 1);
	pthread_mutex_init(&philo_data->meal_mutex, NULL);
	make_threads(philo_data);
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	while (1)
	{
		i = 0;
		while (i < philo_data->philos_count)
		{
			if (check_starvation_inbetween(philo_data->process[i],
					get_time_in_ms()))
			{
				write_status(philo_data->process[i], DEAD);
				kill_the_rest(philo_data);
				return (0);
			}
			i++;
		}
	}
	return (0);
}
