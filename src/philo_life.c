/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/28 13:06:43 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	put_forks_back(t_philo_process *process, int fork1, int fork2)
{
	pthread_mutex_lock(&process->philo_data->fork[fork1].mutex);
	pthread_mutex_lock(&process->philo_data->fork[fork2].mutex);
	process->philo_data->fork[fork1].is_used = 0;
	process->philo_data->fork[fork2].is_used = 0;
	pthread_mutex_unlock(&process->philo_data->fork[fork1].mutex);
	pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
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

void	philo_think_sleep(t_philo_process *process, long int *ideal_time)
{
	if (process->state == THINKING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!process->is_dead)
			printf("%lu: Philosopher %d is thinking\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!process->is_dead)
			printf("%lu: Philosopher %d is sleeping\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		if (starvation_sleeping_check(process))
			return ;
		usleep((process->philo_data->t_to_sleep
				- get_time_in_ms() + *ideal_time) * 1000);
		*ideal_time += process->philo_data->t_to_sleep;
		process->state = THINKING;
	}
}

void	philo_eat(t_philo_process *process, long int *ideal_time)
{
	int	fork1;
	int	fork2;

	fork1 = 0;
	fork2 = 0;
	get_fork_num(process, &fork1, &fork2);
	forks_lock(process, fork1, fork2, ideal_time);
	pthread_mutex_lock(&process->philo_data->log_mutex);
	process->last_meal = get_time_in_ms();
	if (!process->is_dead)
		printf("%lu: Philosopher %d is eating\n", process->last_meal
			- process->philo_data->start_time, process->philo_num + 1);
	pthread_mutex_unlock(&process->philo_data->log_mutex);
	usleep((process->philo_data->t_to_eat
			- get_time_in_ms() + *ideal_time) * 1000);
	*ideal_time += process->philo_data->t_to_eat;
	put_forks_back(process, fork1, fork2);
	process->state = SLEEPING;
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;
	long int		ideal_time;

	process = (t_philo_process *)arg;
	if (process->philo_data->philos_count <= 1)
	{
		usleep((process->philo_data->t_to_die) * 1000);
		process->is_dead = 1;
		process->philo_data->death = 1;
		printf("%lu: Philosopher %d died\n", get_time_in_ms()
			- process->philo_data->start_time, process->philo_num + 1);
		return (NULL);
	}
	ideal_time = process->philo_data->start_time;
	while (!process->is_dead)
	{
		if (process->state == EATING)
			philo_eat(process, &ideal_time);
		else
			philo_think_sleep(process, &ideal_time);
	}
	return (NULL);
}
