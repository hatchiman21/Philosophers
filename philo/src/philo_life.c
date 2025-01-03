/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/01 17:40:08 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	get_fork_num(t_philo_process *process, int *fork1, int *fork2)
{
	int	next_fork;

	if (process->philo_num == 0)
		next_fork = process->philo_data->philos_count - 1;
	else
		next_fork = process->philo_num - 1;
	if (process->philo_num % 2 == 0)
	{
		*fork1 = process->philo_num;
		*fork2 = next_fork;
	}
	else
	{
		*fork1 = next_fork;
		*fork2 = process->philo_num;
	}
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

void	grap_fork(t_philo_process *process, int fork)
{
	while (1)
	{
		pthread_mutex_lock(&process->philo_data->fork[fork].mutex);
		if (process->philo_data->fork[fork].is_used == 0)
		{
			process->philo_data->fork[fork].is_used = 1;
			write_status(process, HELD_FORK);
			pthread_mutex_unlock(&process->philo_data->fork[fork].mutex);
			break ;
		}
		else
		{
			pthread_mutex_unlock(&process->philo_data->fork[fork].mutex);
			if (check_starvation_inbetween(process, get_time_in_ms()))
				break ;
		}
	}
}

void	let_go_of_fork(t_philo_process *process, int fork)
{
	pthread_mutex_lock(&process->philo_data->fork[fork].mutex);
	process->philo_data->fork[fork].is_used = 0;
	pthread_mutex_unlock(&process->philo_data->fork[fork].mutex);
}

void	philo_eat(t_philo_process *process)
{
	grap_fork(process, process->fork1);
	grap_fork(process, process->fork2);
	if (check_starvation_inbetween(process, get_time_in_ms()))
	{
		let_go_of_fork(process, process->fork2);
		let_go_of_fork(process, process->fork1);
		return ;
	}
	write_status(process, EATING);
	pthread_mutex_lock(&process->philo_data->meal_mutex);
	process->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&process->philo_data->meal_mutex);
	my_usleep(process, process->philo_data->t_to_eat);
	let_go_of_fork(process, process->fork2);
	let_go_of_fork(process, process->fork1);
	process->state = SLEEPING;
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;

	process = (t_philo_process *)arg;
	while (process->philo_data->start_time > get_time_in_ms())
		continue ;
	while (1)
	{
		pthread_mutex_lock(&process->philo_data->sim_stop_mutex);
		if (process->philo_data->sim_stop)
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
