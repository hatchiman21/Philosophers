/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/03 23:14:38 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	update_last_meal_and_death_time(t_philo_process *process,
			long *last_meal, long *time_of_death)
{
	sem_wait(process->philo_data->meal_sem);
	*last_meal = process->last_meal;
	sem_post(process->philo_data->meal_sem);
	*time_of_death = process->philo_data->t_to_die + *last_meal;
}

void	*monitor_starvation(void *arg)
{
	t_philo_process	*process;
	long			last_meal;
	long			time_of_death;

	process = (t_philo_process *)arg;
	while (1)
	{
		update_last_meal_and_death_time(process, &last_meal, &time_of_death);
		if (time_of_death - get_time_in_ms() > 10)
			usleep((time_of_death - get_time_in_ms() - 10) * 1000);
		update_last_meal_and_death_time(process, &last_meal, &time_of_death);
		if (time_of_death - get_time_in_ms() > 10)
			continue ;
		while (time_of_death - get_time_in_ms() < 10)
		{
			if (check_starvation(process, get_time_in_ms()))
				return (NULL);
			usleep(50);
		}
	}
	return (NULL);
}

void	philo_eat(t_philo_process *process)
{
	sem_wait(process->philo_data->can_eat);
	sem_wait(process->philo_data->fork);
	write_status(process, HELD_FORK);
	usleep(100);
	sem_wait(process->philo_data->fork);
	write_status(process, HELD_FORK);
	write_status(process, EATING);
	sem_wait(process->philo_data->meal_sem);
	process->meals++;
	if (process->meals == process->philo_data->times_must_eat)
		sem_post(process->philo_data->enugh_meals);
	process->last_meal = get_time_in_ms();
	sem_post(process->philo_data->meal_sem);
	my_usleep(process, process->philo_data->t_to_eat * 1000 - 100);
	sem_post(process->philo_data->fork);
	sem_post(process->philo_data->fork);
	sem_post(process->philo_data->can_eat);
	process->state = SLEEPING;
}

void	philo_think_sleep(t_philo_process *process)
{
	if (process->state == THINKING)
	{
		write_status(process, THINKING);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		write_status(process, SLEEPING);
		my_usleep(process, process->philo_data->t_to_sleep * 1000);
		process->state = THINKING;
	}
}

void	philo_routine(void *arg)
{
	t_philo_process	*process;
	pthread_t		monitor;

	process = (t_philo_process *)arg;
	while (process->philo_data->start_time > get_time_in_ms())
		continue ;
	if (pthread_create(&monitor, NULL, monitor_starvation, process))
	{
		write(2, "thread creation failed\n", 24);
		sem_post(process->philo_data->sim_stop_sem);
		sem_wait(process->philo_data->sim_already_stopped);
		while (1)
			usleep(10000);
	}
	while (1)
	{
		if (process->state == EATING)
			philo_eat(process);
		else
			philo_think_sleep(process);
	}
	while (1)
		usleep(10000);
}
