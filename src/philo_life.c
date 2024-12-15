/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/15 22:50:23 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	philo_think_sleep(t_philo_process *process, unsigned long time_stamp)
{
	if (process->state == THINKING)
	{
		printf("%lu: Philosopher %d is thinking\n", time_stamp,
			process->philo_num + 1);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		printf("%lu: Philosopher %d is sleeping\n", time_stamp,
			process->philo_num + 1);
		usleep(process->philo_data->t_to_sleep);
		process->state = THINKING;
	}
}

void	forks_lock(t_philo_process *process, int next_fork)
{
	if (process->philo_num % 2 == 0)
	{
		pthread_mutex_lock(&process->philo_data->fork[process->philo_num]);
		pthread_mutex_lock(&process->philo_data->fork[next_fork]);
	}
	else
	{
		pthread_mutex_lock(&process->philo_data->fork[next_fork]);
		pthread_mutex_lock(&process->philo_data->fork[process->philo_num]);
	}
}

void	philo_eat(t_philo_process *process, unsigned long time_stamp)
{
	int	next_fork;

	if (process->philo_num == 0)
		next_fork = process->philo_data->philos_count - 1;
	else
		next_fork = process->philo_num - 1;
	forks_lock(process, next_fork);
	time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
	if (check_starvation(process, 1, next_fork) == 1)
		return ;
	printf("%lu: Philosopher %d is eating\n", time_stamp,
		process->philo_num + 1);
	usleep(process->philo_data->t_to_eat);
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
	while (process->is_dead == 0)
	{
		if (check_starvation(process, 0, 0) == 1)
			break ;
		time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
		if (process->state == EATING)
			philo_eat(process, time_stamp);
		else
			philo_think_sleep(process, time_stamp);
	}
	gettimeofday(&process->time, NULL);
	time_stamp = process->time.tv_sec * 1000000 + process->time.tv_usec;
	printf("%lu: Philosopher %d died\n", time_stamp,
			process->philo_num + 1);
	return (NULL);
}
