/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:06 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/27 07:18:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time_in_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	philo_think_sleep(t_philo_process *process)
{
	// unsigned long	time_diff;

	if (process->state == THINKING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!will_starve() && !process->is_killed && !process->is_dead)
			printf("%lu: Philosopher %d is thinking\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		process->state = EATING;
	}
	else if (process->state == SLEEPING)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!will_starve() && !process->is_killed && !process->is_dead)
			printf("%lu: Philosopher %d is sleeping\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		// time_diff = get_time_in_ms() - time_stamp;
		usleep((process->philo_data->t_to_sleep) * 1000);
		process->state = THINKING;
	}
}

void	forks_lock(t_philo_process *process, int next_fork)
{
	if (process->philo_num % 2 == 0 && process->philo_data->philos_count > 1
		&& process->is_killed == 0)
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

void	philo_eat(t_philo_process *process)
{
	int				next_fork;
	// unsigned long	time_diff;

	if (process->philo_num == 0)
		next_fork = process->philo_data->philos_count - 1;
	else
		next_fork = process->philo_num - 1;
	forks_lock(process, next_fork);
	if (process->philo_data->philos_count <= 1
		|| process->is_killed == 1
		|| check_starvation(process, 1, next_fork) == 1)
		return ;
	process->last_meal = get_time_in_ms();
	pthread_mutex_lock(&process->philo_data->log_mutex);
	if (!process->is_killed && !process->is_dead)
		printf("%lu: Philosopher %d is eating\n", process->last_meal
			- process->philo_data->start_time, process->philo_num + 1);
	pthread_mutex_unlock(&process->philo_data->log_mutex);
	// time_diff = get_time_in_ms() - time_stamp;
	// if (time_diff < process->philo_data->t_to_eat)
		usleep((process->philo_data->t_to_eat) * 1000);
	pthread_mutex_unlock(&process->philo_data->fork[process->philo_num]);
	pthread_mutex_unlock(&process->philo_data->fork[next_fork]);
	process->state = SLEEPING;
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;
	// unsigned long	time_stamp;

	process = (t_philo_process *)arg;
	process->last_meal = get_time_in_ms();
	while (process->is_killed == 0)
	{
		if (check_starvation(process, 0, 0) == 1)
			break ;
		// time_stamp = get_time_in_ms() - process->philo_data->start_time;
		if (process->state == EATING)
			philo_eat(process);
		else
			philo_think_sleep(process);
	}
		return (NULL);
}
