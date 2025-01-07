/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intiate_philo_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 05:52:22 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/07 18:52:13 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	creat_processes(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	while (i < philo_data->philos_count)
	{
		process = philo_data->process[i];
		process->id = fork();
		if (process->id < 0)
			philo_error_handling(philo_data, i, 6);
		if (!process->id)
			philo_routine(process);
		i++;
	}
}

void	allocate_philos(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	philo_data->start_time = get_time_in_ms() + (philo_data->philos_count * 20);
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, i, 5);
		process = philo_data->process[i];
		process->philo_num = i;
		process->philo_data = philo_data;
		process->state = EATING;
		process->last_meal = philo_data->start_time;
		process->meals = 0;
		i++;
	}
}

void	initiate_philo_sem(t_philo *philo_data)
{
	sem_unlink("log");
	sem_unlink("sim_stop");
	sem_unlink("fork");
	sem_unlink("can_eat");
	sem_unlink("sim_already_stopped");
	sem_unlink("meal");
	sem_unlink("enugh_meals");
	philo_data->can_eat = sem_open("can_eat", O_CREAT, 0644,
			philo_data->philos_count / 2 + philo_data->philos_count % 2);
	philo_data->sim_already_stopped = sem_open("sim_already_stopped",
			O_CREAT, 0644, 1);
	philo_data->enugh_meals = sem_open("enugh_meals", O_CREAT, 0644, 0);
	philo_data->meal_sem = sem_open("meal", O_CREAT, 0644, 1);
	philo_data->log_sem = sem_open("log", O_CREAT, 0644, 1);
	philo_data->sim_stop_sem = sem_open("sim_stop", O_CREAT, 0644, 0);
	philo_data->fork = sem_open("fork", O_CREAT, 0644,
			philo_data->philos_count);
	if (philo_data->fork == SEM_FAILED || philo_data->log_sem == SEM_FAILED
		|| philo_data->sim_stop_sem == SEM_FAILED
		|| philo_data->meal_sem == SEM_FAILED
		|| philo_data->can_eat == SEM_FAILED
		|| philo_data->enugh_meals == SEM_FAILED
		|| philo_data->sim_already_stopped == SEM_FAILED)
		philo_error_handling(philo_data, 0, 4);
}

void	get_values(t_philo *philo_data, char *argv[], int argc)
{
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		philo_data->times_must_eat = ft_atoi(argv[5]);
	else
		philo_data->times_must_eat = 0;
}

t_philo	*assign_philo_data(char *argv[], int argc)
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
	philo_data->sim_stop = 0;
	return (philo_data);
}
