/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_starvation_checks.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:44:24 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/28 23:01:49 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_starvation_inbetween(t_philo_process *process, long time_now)
{
	if (process->is_dead)
		return (1);
	if (time_now - process->last_meal > process->philo_data->t_to_die)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		pthread_mutex_lock(&process->philo_data->death_mutex);
		pthread_mutex_lock(&process->is_dead_mutex);
		if (!process->is_dead)
		{
			printf("%lu: Philosopher %d died\n", time_now
				- process->philo_data->start_time, process->philo_num + 1);
			process->philo_data->death = 1;
			process->is_dead = 1;
			usleep(10000);
		}
		pthread_mutex_unlock(&process->is_dead_mutex);
		pthread_mutex_unlock(&process->philo_data->death_mutex);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		return (1);
	}
	return (0);
}

int	my_usleep(t_philo_process *process, int time)
{
	long	start_time;

	start_time = get_time_in_ms();
	while (get_time_in_ms() - start_time < time)
	{
		if (check_starvation_inbetween(process, get_time_in_ms()))
			return (1);
	}
	return (0);
}

// int	starvation_sleeping_check(t_philo_process *process)
// {
// 	int		time_sleeping;
// 	int		time_left_to_die;
// 	long	time_now;

// 	time_now = get_time_in_ms();
// 	time_sleeping = time_now + process->philo_data->t_to_sleep;
// 	time_left_to_die = process->last_meal + process->philo_data->t_to_die;
// 	if (time_sleeping >= time_left_to_die)
// 	{
// 		if (!process->is_dead)
// 		{
// 			usleep((time_left_to_die - time_now) * 1000);
// 			pthread_mutex_lock(&process->philo_data->log_mutex);
// 			if (!process->is_dead)
// 			{
// 				printf("%lu: Philosopher %d died\n", get_time_in_ms()
// 					- process->philo_data->start_time, process->philo_num + 1);
// 				process->philo_data->death = 1;
// 				process->is_dead = 1;
// 				usleep(10000);
// 			}
// 			pthread_mutex_unlock(&process->philo_data->log_mutex);
// 		}
// 		return (1);
// 	}
// 	return (0);
// }
