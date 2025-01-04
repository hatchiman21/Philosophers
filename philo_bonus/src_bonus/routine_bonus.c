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

void	philo_eat(t_philo_process *process)
{
	grap_fork(process, process->fork1);
	grap_fork(process, process->fork2);
	if (check_starvation(process, get_time_in_ms()))
	{
		let_go_of_fork(process, process->fork2);
		let_go_of_fork(process, process->fork1);
		return ;
	}
	write_status(process, EATING);
	pthread_mutex_lock(&process->philo_data->meal_mutex);
	process->meals++;
	process->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&process->philo_data->meal_mutex);
	my_usleep(process, process->philo_data->t_to_eat);
	let_go_of_fork(process, process->fork2);
	let_go_of_fork(process, process->fork1);
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
		my_usleep(process, process->philo_data->t_to_sleep);
		process->state = THINKING;
	}
}

void	*routine(void *arg)
{
	t_philo_process	*process;

	process = (t_philo_process *)arg;
	while (process->philo_data->start_time > get_time_in_ms())
		continue ;
	while (1)
	{
		pthread_mutex_lock(&process->philo_data->sim_stop_mutex);
		if (process->philo_data->sim_stop
			|| check_starvation(process, get_time_in_ms()))
		{
			pthread_mutex_unlock(&process->philo_data->sim_stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&process->philo_data->sim_stop_mutex);
		if (process->state == EATING)
			philo_eat(process);
		else
			philo_think_sleep(process);
	}
	return (NULL);
}
