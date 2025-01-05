/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:12:55 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/05 07:31:23 by aatieh           ###   ########.fr       */
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
		write(2, "log sem failed\n", 16);
	else if (error == 6)
		write(2, "sim_stop sem failed\n", 21);
	else if (error == 7)
		write(2, "meal sem failed\n", 17);
	else if (error == 8)
		write(2, "fork sem failed\n", 17);
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
		kill(0, SIGKILL);
	}
	i = 0;
	while (error > 9 && i < philo->philos_count)
		free(philo->process[i++]);
	while (error == 9 && i < num)
		free(philo->process[i++]);
	if (error >= 8)
	{
		// sem_destroy(philo->meal_sem);
		sem_destroy(philo->sim_stop_sem);
	}
	if (error >= 7)
		sem_destroy(philo->sim_stop_sem);
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
					sem_destroy(philo->log_sem);
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
