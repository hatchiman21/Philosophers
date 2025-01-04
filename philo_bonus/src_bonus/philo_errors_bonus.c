/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:12:55 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 08:14:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	philo_error_message(int error)
{
	if (error == 1)
		write(2, "wrong input format\n", 20);
	else if (error == 2)
		write(2, "main philo malloc failed\n", 26);
	else if (error == 3)
		write(2, "pthread process malloc failed\n", 31);
	else if (error == 4)
		write(2, "pthread fork malloc failed\n", 28);
	else if (error == 5)
		write(2, "log mutex failed\n", 18);
	else if (error == 6)
		write(2, "sim_stop mutex failed\n", 23);
	else if (error == 7)
		write(2, "meal mutex failed\n", 19);
	else if (error == 8)
		write(2, "fork mutex failed\n", 19);
	else if (error == 9)
		write(2, "process malloc failed\n", 23);
	else if (error == 10)
		write(2, "thread creation failed\n", 24);
}

void	philo_thread_error_handling(t_philo *philo, int num, int error)
{
	int	i;

	if (error == 10)
	{
		pthread_mutex_lock(&philo->sim_stop_mutex);
		philo->sim_stop = 1;
		pthread_mutex_unlock(&philo->sim_stop_mutex);
		i = 0;
		while (i <= num)
			pthread_join(philo->process[i++]->thread, NULL);
	}
	i = 0;
	while (error > 9 && i < philo->philos_count)
		free(philo->process[i++]);
	while (error == 9 && i < num)
		free(philo->process[i++]);
	if (error >= 8)
		pthread_mutex_destroy(&philo->meal_mutex);
	i = 0;
	while (error == 8 && (i < num))
		pthread_mutex_destroy(&philo->fork[i++].mutex);
	while (error > 8 && (i < philo->philos_count))
		pthread_mutex_destroy(&philo->fork[i++].mutex);
	if (error >= 7)
		pthread_mutex_destroy(&philo->sim_stop_mutex);
}

void	philo_error_handling(t_philo *philo, int num, int error)
{
	if (error >= 3)
	{
		if (error >= 4)
		{
			if (error >= 5)
			{
				if (error >= 6)
				{
					philo_thread_error_handling(philo, num, error);
					pthread_mutex_destroy(&philo->log_mutex);
				}
				free(philo->fork);
			}
			free(philo->process);
		}
		free(philo);
	}
	philo_error_message(error);
	exit(error);
}
