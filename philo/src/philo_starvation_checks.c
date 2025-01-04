/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_starvation_checks.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:44:24 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 20:20:54 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_starvation_and_meals(t_philo_process *process,
		t_philo *philo_data, long time_now, int *eaten_enough)
{
	pthread_mutex_lock(&philo_data->meal_mutex);
	if (time_now - process->last_meal > philo_data->t_to_die)
	{
		pthread_mutex_unlock(&philo_data->meal_mutex);
		return (1);
	}
	if (process->meals < philo_data->times_must_eat)
		*eaten_enough = 0;
	pthread_mutex_unlock(&philo_data->meal_mutex);
	return (0);
}

int	check_starvation(t_philo_process *process,
		t_philo *philo_data, long time_now)
{
	pthread_mutex_lock(&philo_data->meal_mutex);
	if (time_now - process->last_meal > philo_data->t_to_die)
	{
		pthread_mutex_unlock(&philo_data->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo_data->meal_mutex);
	return (0);
}

int	my_usleep(t_philo *philo_data, int time)
{
	long	start_time;

	start_time = get_time_in_ms();
	while (get_time_in_ms() - start_time < time)
	{
		pthread_mutex_lock(&philo_data->sim_stop_mutex);
		if (philo_data->sim_stop)
		{
			pthread_mutex_unlock(&philo_data->sim_stop_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo_data->sim_stop_mutex);
		usleep(100);
	}
	return (0);
}
