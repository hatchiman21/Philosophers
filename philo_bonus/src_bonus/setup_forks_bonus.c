/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_forks_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 05:49:14 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 08:14:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	get_fork_num(t_philo_process *process, int *fork1, int *fork2)
{
	int	left_fork;
	int	right_fork;

	right_fork = process->philo_num;
	if (process->philo_num == 0)
		left_fork = process->philo_data->philos_count - 1;
	else
		left_fork = process->philo_num - 1;
	if (process->philo_num % 2 == 0)
	{
		*fork1 = right_fork;
		*fork2 = left_fork;
	}
	else
	{
		*fork1 = left_fork;
		*fork2 = right_fork;
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
			if (check_starvation(process, get_time_in_ms()))
				break ;
		}
		usleep(100);
	}
}

void	let_go_of_fork(t_philo_process *process, int fork)
{
	pthread_mutex_lock(&process->philo_data->fork[fork].mutex);
	process->philo_data->fork[fork].is_used = 0;
	pthread_mutex_unlock(&process->philo_data->fork[fork].mutex);
}
