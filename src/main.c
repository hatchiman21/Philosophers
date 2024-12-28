/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/28 23:06:33 by aatieh           ###   ########.fr       */
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
		pthread_mutex_lock(&philos->process[i]->is_dead_mutex);
		philos->process[i++]->is_dead = 1;
		pthread_mutex_unlock(&philos->process[i - 1]->is_dead_mutex);
	}
	i = 0;
	while (i < philos->philos_count)
	{
		pthread_join(philos->process[i]->thread, NULL);
		free(philos->process[i++]);
	}
	i = 0;
	while (i < philos->philos_count)
		pthread_mutex_destroy(&philos->fork[i++].mutex);
	pthread_mutex_destroy(&philos->log_mutex);
	free(philos->process);
	free(philos->fork);
	free(philos);
}

void	make_threads(t_philo *philo_data)
{
	int	i;

	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, i, 7);
		philo_data->process[i]->philo_num = i;
		philo_data->process[i]->philo_data = philo_data;
		philo_data->process[i]->is_dead = 0;
		get_fork_num(philo_data->process[i],
			&philo_data->process[i]->fork1, &philo_data->process[i]->fork2);
		pthread_mutex_init(&philo_data->process[i]->is_dead_mutex, NULL);
		philo_data->process[i]->state = EATING;
		philo_data->process[i]->fork1_check = 0;
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
	philo_data->start_time = get_time_in_ms();
	size = sizeof(t_philo_process *) * philo_data->philos_count;
	philo_data->process = NULL;
	philo_data->process = malloc(size);
	if (!philo_data->process)
		philo_error_handling(philo_data, 0, 3);
	pthread_mutex_init(&philo_data->death_mutex, NULL);
	philo_data->death = 0;
	philo_data->fork = NULL;
	size = sizeof(t_eating_fork) * philo_data->philos_count;
	philo_data->fork = malloc(size);
	if (!philo_data->fork)
		philo_error_handling(philo_data, 0, 4);
	return (philo_data);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_philo	*philo_data;

	check_input(argv, argc);
	philo_data = assign_philo(argv);
	if (pthread_mutex_init(&philo_data->log_mutex, NULL) != 0)
		philo_error_handling(philo_data, 0, 5);
	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->fork[i].is_used = 0;
		if (pthread_mutex_init(&philo_data->fork[i++].mutex, NULL) != 0)
			philo_error_handling(philo_data, i - 1, 6);
	}
	make_threads(philo_data);
	while (1)
	{
		pthread_mutex_lock(&philo_data->death_mutex);
		if (philo_data->death)
		{
			pthread_mutex_unlock(&philo_data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo_data->death_mutex);
	}
	kill_the_rest(philo_data);
	return (0);
}
