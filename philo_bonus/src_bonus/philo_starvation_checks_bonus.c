/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_starvation_checks_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:44:24 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/05 07:43:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	check_starvation_and_meals(t_philo_process *process,
		t_philo *philo_data, long time_now, int *eaten_enough)
{
	// sem_wait(process->meal_sem);
	if (time_now - process->last_meal > philo_data->t_to_die)
	{
		// sem_post(process->meal_sem);
		return (0);
	}
	if (process->meals < philo_data->times_must_eat)
		*eaten_enough = 0;
	// sem_post(process->meal_sem);
	return (0);
}

int	check_starvation(t_philo_process *process, long time_now)
{
	sem_wait(process->meal_sem);
	if (time_now - process->last_meal > process->philo_data->t_to_die)
	{
		sem_post(process->meal_sem);
		write_status(process, DEAD);
		return (1);
	}
	sem_post(process->meal_sem);
	return (0);
}

int	my_usleep(t_philo_process *process, int time)
{
	long	start_time;

	start_time = get_time_in_ms();
	while (get_time_in_ms() - start_time < time)
	{
		if (check_starvation(process, get_time_in_ms()))
			return (1);
		usleep(50);
	}
	return (0);
}
