/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:48 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/28 13:05:28 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		c = 2048;
	else
		c = 0;
	return (c);
}

long	get_time_in_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

long	ft_atoi(const char *str)
{
	int			i;
	long int	res;
	int			sign;

	i = 0;
	sign = 1;
	res = 0;
	while (((str[i] >= 9 && str[i] <= 13) || str[i] == ' ') && str[i])
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res += str[i++] - '0';
		if (str[i] && str[i] >= '0' && str[i] <= '9')
			res *= 10;
	}
	return (res * sign);
}

void	forks_lock(t_philo_process *process, int fork1,
		int fork2, long *ideal_time)
{
	while (!process->is_dead)
	{
		pthread_mutex_lock(&process->philo_data->fork[fork1].mutex);
		pthread_mutex_lock(&process->philo_data->fork[fork2].mutex);
		if (process->philo_data->fork[fork1].is_used == 0
			&& process->philo_data->fork[fork2].is_used == 0)
		{
			*ideal_time += get_time_in_ms() - *ideal_time;
			process->philo_data->fork[fork1].is_used = 1;
			process->philo_data->fork[fork2].is_used = 1;
			pthread_mutex_unlock(&process->philo_data->fork[fork1].mutex);
			pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
			break ;
		}
		pthread_mutex_unlock(&process->philo_data->fork[fork1].mutex);
		pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
		if (check_starvation_inbetween(process))
			return ;
	}
}
