/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intiate_philo_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 05:52:22 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 20:58:39 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	creat_threads(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	while (i < philo_data->philos_count)
	{
		process = philo_data->process[i];
		process->id = fork();
		if (!process->id)
			routine(process);
		i++;
	}
}

void	create_processes(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	philo_data->start_time = get_time_in_ms() + (philo_data->philos_count * 10);
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, i, 9);
		process = philo_data->process[i];
		process->philo_num = i;
		process->philo_data = philo_data;
		process->state = EATING;
		process->last_meal = philo_data->start_time;
		process->meals = 0;
		i++;
	}
	creat_threads(philo_data);
}

void	intiate_philo_seph(t_philo *philo_data)
{
	philo_data->log_mutex = sem_open("log", O_CREAT, 0644, 1);
	philo_data->sim_stop_mutex = sem_open("sim_stop", O_CREAT, 0644, 1);
	philo_data->meal_mutex = sem_open("meal", O_CREAT, 0644, 1);
	// if (pthread_mutex_init(&philo_data->log_mutex, NULL) != 0)
	// 	philo_error_handling(philo_data, 0, 5);
	// if (pthread_mutex_init(&philo_data->sim_stop_mutex, NULL) != 0)
	// 	philo_error_handling(philo_data, 0, 6);
	// if (pthread_mutex_init(&philo_data->meal_mutex, NULL) != 0)
	// 	philo_error_handling(philo_data, 0, 7);
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
		philo_error_handling(NULL, 0, 2);
	get_values(philo_data, argv, argc);
	philo_data->process = NULL;
	philo_data->process = malloc(sizeof(t_philo_process *)
			* philo_data->philos_count);
	if (!philo_data->process)
		philo_error_handling(philo_data, 0, 3);
	philo_data->fork = sem_open(philo_data->philos_count, O_CREAT, 0644, philo_data->philos_count);
	// philo_data->fork = NULL;
	// philo_data->fork = malloc(sizeof(t_eating_fork)
	// 		* philo_data->philos_count);
	// if (!philo_data->fork)
	// 	philo_error_handling(philo_data, 0, 4);
	intiate_philo_seph(philo_data);
	philo_data->sim_stop = 0;
	return (philo_data);
}
