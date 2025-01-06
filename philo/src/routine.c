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

#include "../philo.h"

void	philo_eat(t_philo_process *process, t_philo *philo_data)
{
	grap_fork(process, philo_data, process->fork1);
	grap_fork(process, philo_data, process->fork2);
	if (check_starvation(process, philo_data, get_time_in_ms()))
	{
		let_go_of_fork(philo_data, process->fork2);
		let_go_of_fork(philo_data, process->fork1);
		return ;
	}
	write_status(process, philo_data, EATING);
	pthread_mutex_lock(&philo_data->meal_mutex);
	process->meals++;
	process->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo_data->meal_mutex);
	my_usleep(philo_data, philo_data->t_to_eat * 1000);
	let_go_of_fork(philo_data, process->fork2);
	let_go_of_fork(philo_data, process->fork1);
	process->state = SLEEPING;
}

void	philo_think_sleep(t_philo_process *process)
{
	if (process->state == THINKING)
	{
		write_status(process, process->philo_data, THINKING);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		write_status(process, process->philo_data, SLEEPING);
		my_usleep(process->philo_data, process->philo_data->t_to_sleep * 1000);
		process->state = THINKING;
	}
}

void	*routine(void *arg)
{
	t_philo_process	*process;
	t_philo			*philo_data;

	process = (t_philo_process *)arg;
	philo_data = process->philo_data;
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	while (1)
	{
		pthread_mutex_lock(&philo_data->sim_stop_mutex);
		if (philo_data->sim_stop
			|| check_starvation(process, philo_data, get_time_in_ms()))
		{
			pthread_mutex_unlock(&philo_data->sim_stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo_data->sim_stop_mutex);
		if (process->state == EATING)
			philo_eat(process, philo_data);
		else
			philo_think_sleep(process);
	}
	return (NULL);
}
