/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intiate_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 05:52:22 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/26 03:16:52 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*creat_threads(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	process = NULL;
	while (i < philo_data->philos_count)
	{
		process = philo_data->process[i];
		if (pthread_create(&process->thread, NULL, &routine, process) != 0)
			return (philo_error_handling(philo_data, i, 10));
		i++;
	}
	return (process);
}

void	*make_threads(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	philo_data->start_time = get_time_in_ms() + (philo_data->philos_count * 10);
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			return (philo_error_handling(philo_data, i, 9));
		process = philo_data->process[i];
		process->philo_num = i;
		process->philo_data = philo_data;
		get_fork_num(process,
			&process->fork1, &process->fork2);
		process->state = EATING;
		process->last_meal = philo_data->start_time;
		process->meals = 0;
		i++;
	}
	return (creat_threads(philo_data));
}

void	*intiate_philo_mutex(t_philo *philo_data)
{
	int	i;

	if (pthread_mutex_init(&philo_data->log_mutex, NULL) != 0)
		return (philo_error_handling(philo_data, 0, 5));
	i = 0;
	if (pthread_mutex_init(&philo_data->sim_stop_mutex, NULL) != 0)
		return (philo_error_handling(philo_data, 0, 6));
	if (pthread_mutex_init(&philo_data->meal_mutex, NULL) != 0)
		return (philo_error_handling(philo_data, 0, 7));
	while (i < philo_data->philos_count)
	{
		philo_data->fork[i].is_used = 0;
		if (pthread_mutex_init(&philo_data->fork[i++].mutex, NULL) != 0)
			return (philo_error_handling(philo_data, i - 1, 8));
	}
	return (philo_data);
}

void	get_values(t_philo *philo_data, char *argv[], int argc)
{
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		philo_data->times_must_eat = ft_atoi(argv[5]);
		philo_data->limited_meals = 1;
	}
	else
	{
		philo_data->times_must_eat = 0;
		philo_data->limited_meals = 0;
	}
}

t_philo	*assign_philo(char *argv[], int argc)
{
	t_philo			*philo_data;

	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		return (philo_error_handling(NULL, 0, 2));
	get_values(philo_data, argv, argc);
	philo_data->process = NULL;
	philo_data->process = malloc(sizeof(t_philo_process *)
			* philo_data->philos_count);
	if (!philo_data->process)
		return (philo_error_handling(philo_data, 0, 3));
	philo_data->fork = NULL;
	philo_data->fork = malloc(sizeof(t_eating_fork)
			* philo_data->philos_count);
	if (!philo_data->fork)
		return (philo_error_handling(philo_data, 0, 4));
	if (!intiate_philo_mutex(philo_data))
		return (NULL);
	philo_data->sim_stop = 0;
	return (philo_data);
}
