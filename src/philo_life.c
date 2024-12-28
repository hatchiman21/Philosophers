/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/28 23:04:47 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	put_forks_back(t_philo_process *process, int fork1, int fork2)
{
	pthread_mutex_lock(&process->philo_data->fork[fork1].mutex);
	pthread_mutex_lock(&process->philo_data->fork[fork2].mutex);
	process->philo_data->fork[fork1].is_used = 0;
	process->philo_data->fork[fork2].is_used = 0;
	process->fork1_check = 0;
	pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
	pthread_mutex_unlock(&process->philo_data->fork[fork1].mutex);
}

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
		pthread_mutex_lock(&process->is_dead_mutex);
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!process->is_dead)
			printf("%lu: Philosopher %d is thinking\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		process->state = EATING;
		pthread_mutex_unlock(&process->is_dead_mutex);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
	}
	else if (process->state == SLEEPING)
	{
		pthread_mutex_lock(&process->is_dead_mutex);
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!process->is_dead)
			printf("%lu: Philosopher %d is sleeping\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		pthread_mutex_unlock(&process->is_dead_mutex);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		my_usleep(process, process->philo_data->t_to_sleep);
		process->state = THINKING;
	}
}

void	philo_eat(t_philo_process *process)
{
	forks_lock(process, process->fork1, process->fork2);
	pthread_mutex_lock(&process->is_dead_mutex);
	pthread_mutex_lock(&process->philo_data->log_mutex);
	process->last_meal = get_time_in_ms();
	if (!process->is_dead)
		printf("%lu: Philosopher %d is eating\n", process->last_meal
			- process->philo_data->start_time, process->philo_num + 1);
	pthread_mutex_unlock(&process->is_dead_mutex);
	pthread_mutex_unlock(&process->philo_data->log_mutex);
	my_usleep(process, process->philo_data->t_to_eat);
	put_forks_back(process, process->fork1, process->fork2);
	process->state = SLEEPING;
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;

	process = (t_philo_process *)arg;
	while (1)
	{
		if (process->state == EATING)
			philo_eat(process);
		else
			philo_think_sleep(process);
		pthread_mutex_lock(&process->is_dead_mutex);
		if (process->is_dead)
		{
			pthread_mutex_unlock(&process->is_dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&process->is_dead_mutex);
	}
	return (NULL);
}
