/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 20:17:34 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	write_message(t_philo_process *process, char *msg)
{
	printf("%lu ms Philosopher %d %s\n", get_time_in_ms()
		- process->philo_data->start_time, process->philo_num + 1, msg);
}

void	write_status(t_philo_process *process, t_philo *philo_data, int status)
{
	pthread_mutex_lock(&philo_data->log_mutex);
	pthread_mutex_lock(&philo_data->sim_stop_mutex);
	if (philo_data->sim_stop)
	{
		pthread_mutex_unlock(&philo_data->sim_stop_mutex);
		pthread_mutex_unlock(&philo_data->log_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo_data->sim_stop_mutex);
	if (status == HELD_FORK)
		write_message(process, "has taken a fork");
	else if (status == EATING)
		write_message(process, "is eating");
	else if (status == THINKING)
		write_message(process, "is thinking");
	else if (status == SLEEPING)
		write_message(process, "is sleeping");
	else if (status == DEAD)
		write_message(process, "died");
	pthread_mutex_unlock(&philo_data->log_mutex);
}

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
