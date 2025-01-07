/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_errors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:12:55 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/07 18:53:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	philo_error_message(int error)
{
	if (error == 0)
		write(2, "wrong input format\n", 20);
	else if (error == 1)
		write(2, "invalid input values\n", 22);
	else if (error == 2)
		write(2, "main philo malloc failed\n", 26);
	else if (error == 3)
		write(2, "pthread process malloc failed\n", 31);
	else if (error == 4)
		write(2, "semaphore creation failed\n", 27);
	else if (error == 5)
		write(2, "process malloc failed\n", 23);
	else if (error == 6)
		write(2, "fork failed\n", 13);
	else if (error == 7)
		write(2, "thread creation failed\n", 24);
}

void	philo_thread_error_handling(t_philo *philo, int num, int error)
{
	int	i;

	i = 0;
	while (error == 6 && i < num)
		kill(philo->process[i++]->id, SIGKILL);
	while (error > 6 && i < philo->philos_count)
		kill(philo->process[i++]->id, SIGKILL);
	i = 0;
	while (error > 5 && i < philo->philos_count)
		free(philo->process[i++]);
	while (error == 5 && i < num)
		free(philo->process[i++]);
}

void	philo_sem_clean(t_philo *philo_data)
{
	if (philo_data->fork != SEM_FAILED)
		sem_close(philo_data->fork);
	if (philo_data->log_sem != SEM_FAILED)
		sem_close(philo_data->log_sem);
	if (philo_data->sim_stop_sem != SEM_FAILED)
		sem_close(philo_data->sim_stop_sem);
	if (philo_data->can_eat != SEM_FAILED)
		sem_close(philo_data->can_eat);
	if (philo_data->sim_already_stopped != SEM_FAILED)
		sem_close(philo_data->sim_already_stopped);
	if (philo_data->meal_sem != SEM_FAILED)
		sem_close(philo_data->meal_sem);
	if (philo_data->enugh_meals != SEM_FAILED)
		sem_close(philo_data->enugh_meals);
	sem_unlink("log");
	sem_unlink("sim_stop");
	sem_unlink("can_eat");
	sem_unlink("sim_already_stopped");
	sem_unlink("fork");
	sem_unlink("meal");
	sem_unlink("enugh_meals");
}

void	philo_error_handling(t_philo *philo, int num, int error)
{
	if (error >= 3)
	{
		if (error >= 4)
		{
			if (error >= 5)
				philo_thread_error_handling(philo, num, error);
			philo_sem_clean(philo);
			free(philo->process);
		}
		free(philo);
	}
	philo_error_message(error);
	exit(error + 1);
}
