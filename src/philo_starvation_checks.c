/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_starvation_checks.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 21:44:24 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/27 21:56:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	starvation_sleeping_check(t_philo_process *process)
{
	int		time_sleeping;
	int		time_left_to_die;
	long	time_now;

	time_now = get_time_in_ms();
	time_sleeping = time_now + process->philo_data->t_to_sleep;
	time_left_to_die = process->last_meal + process->philo_data->t_to_die;
	if (time_sleeping >= time_left_to_die)
	{
		if (!process->is_killed)
		{
			usleep((time_left_to_die - time_now) * 1000);
			pthread_mutex_lock(&process->philo_data->log_mutex);
			printf("%lu: Philosopher %d died\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
			usleep(2000);
			pthread_mutex_unlock(&process->philo_data->log_mutex);
		}
		exit(0);
	}
}

long	check_waiting(int philo_num, int dir,
		long time_to_die, t_philo *philo_data)
{
	long	time_to_wait;
	int		next_philo;

	if (dir == RIGHT && philo_num == 0)
		next_philo = philo_data->philos_count - 1;
	else if (dir == RIGHT)
		next_philo = philo_num - 1;
	else if (dir == LEFT && philo_num == philo_data->philos_count - 1)
		next_philo = 0;
	else
		next_philo = philo_num + 1;
	time_to_wait = philo_data->process[next_philo]->last_meal
		+ philo_data->t_to_eat;
	if (time_to_wait >= time_to_die)
	{
		time_to_wait = time_to_die - get_time_in_ms();
		return (time_to_wait);
	}
	return (-1);
}

int	check_starvation_while_wating(t_philo *philo_data, int philo_num)
{
	long	time_to_die;
	long	time_to_wait;

	time_to_die = philo_data->process[philo_num]->last_meal
		+ philo_data->t_to_die;
	time_to_wait = check_waiting(philo_num, RIGHT, time_to_die, philo_data);
	if (time_to_wait == -1)
		time_to_wait = check_waiting(philo_num, LEFT, time_to_die, philo_data);
	if (time_to_wait == -1)
		return (0);
	usleep(time_to_wait * 1000);
	pthread_mutex_lock(&philo_data->log_mutex);
	if (!philo_data->process[philo_num]->is_killed)
	{
		printf("%lu: Philosopher %d died\n", get_time_in_ms()
			- philo_data->start_time, philo_num + 1);
		philo_data->process[philo_num]->is_dead = 1;
		philo_data->death = 1;
		usleep(2000);
	}
	pthread_mutex_unlock(&philo_data->log_mutex);
	return (1);
}
// int	check_starvation_inbetween(t_philo_process *process,
// 		int holding_forks, int next_fork)
// {
// 	long			time_diff;
// 	int				fork;

// 	fork = process->philo_num;
// 	time_diff = get_time_in_ms() - process->last_meal;
// 	if (time_diff >= process->philo_data->t_to_die)
// 	{
// 		if (holding_forks)
// 		{
// 			pthread_mutex_unlock(&process->philo_data->fork[fork]);
// 			pthread_mutex_unlock(&process->philo_data->fork[next_fork]);
// 		}
// 		pthread_mutex_lock(&process->philo_data->log_mutex);
// 		if (!process->is_killed && !process->is_dead)
// 			printf("%lu: Philosopher %d died\n", get_time_in_ms()
// 				- process->philo_data->start_time, process->philo_num + 1);
// 		usleep(2000);
// 		pthread_mutex_unlock(&process->philo_data->log_mutex);
// 		process->philo_data->death = 1;
// 		process->is_dead = 1;
// 		return (1);
// 	}
// 	return (0);
// }
