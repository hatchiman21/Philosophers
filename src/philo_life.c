/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/16 06:46:16 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	philo_think_sleep(t_philo_process *process, unsigned long time_stamp)
{
	struct timeval	time;
	unsigned long	time_diff;

	if (process->state == THINKING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		printf("%lu: Philosopher %d is thinking\n", time_stamp,
			process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		printf("%lu: Philosopher %d is sleeping\n", time_stamp,
			process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		gettimeofday(&time, NULL);
		time_diff = time.tv_sec * 1000000 + time.tv_usec - time_stamp;
		if (time_diff < process->philo_data->t_to_sleep)
			usleep(process->philo_data->t_to_sleep - time_diff);
		process->state = THINKING;
	}
}

void	forks_lock(t_philo_process *process, int next_fork)
{
	if (process->philo_num % 2 == 0 && process->philo_data->philos_count > 1 && process->is_killed == 0)
	{
		pthread_mutex_lock(&process->philo_data->fork[process->philo_num]);
		pthread_mutex_lock(&process->philo_data->fork[next_fork]);
	}
	else if (process->philo_data->philos_count > 1 && process->is_killed == 0)
	{
		pthread_mutex_lock(&process->philo_data->fork[next_fork]);
		pthread_mutex_lock(&process->philo_data->fork[process->philo_num]);
	}
}

void	philo_eat(t_philo_process *process, unsigned long time_stamp)
{
	int				next_fork;
	struct timeval	time;	
	unsigned long	time_diff;

	if (process->philo_num == 0)
		next_fork = process->philo_data->philos_count - 1;
	else
		next_fork = process->philo_num - 1;
	forks_lock(process, next_fork);
	time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
	if (process->philo_data->philos_count <= 1 || process->is_killed == 1
		|| check_starvation(process, 1, next_fork) == 1)
		return ;
	pthread_mutex_lock(&process->philo_data->log_mutex);
	printf("%lu: Philosopher %d is eating\n", time_stamp,
		process->philo_num + 1);
	pthread_mutex_unlock(&process->philo_data->log_mutex);
	gettimeofday(&time, NULL);
	time_diff = time.tv_sec * 1000000 + time.tv_usec - time_stamp;
	if (time_diff < process->philo_data->t_to_eat)
		usleep(process->philo_data->t_to_eat - time_diff);
	pthread_mutex_unlock(&process->philo_data->fork[process->philo_num]);
	pthread_mutex_unlock(&process->philo_data->fork[next_fork]);
	gettimeofday(&process->last_meal, NULL);
	process->state = SLEEPING;
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;
	unsigned long	time_stamp;

	process = (t_philo_process *)arg;
	gettimeofday(&process->last_meal, NULL);
	time_stamp = 0;
	while (process->is_killed == 0)
	{
		if (check_starvation(process, 0, 0) == 1)
			break ;
		time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
		if (process->state == EATING)
			philo_eat(process, time_stamp);
		else
			philo_think_sleep(process, time_stamp);
	}
	if (process->is_dead == 1)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		gettimeofday(&process->time, NULL);
		time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
		printf("%lu: Philosopher %d died\n", time_stamp,
			process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
	}
	return (NULL);
}
