/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 08:14:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	write_message(t_philo_process *philo, char *msg)
{
	printf("%lu ms Philosopher %d %s\n", get_time_in_ms()
		- philo->philo_data->start_time, philo->philo_num + 1, msg);
}

void	write_status(t_philo_process *philo, int status)
{
	pthread_mutex_lock(&philo->philo_data->log_mutex);
	pthread_mutex_lock(&philo->philo_data->sim_stop_mutex);
	if (philo->philo_data->sim_stop)
	{
		pthread_mutex_unlock(&philo->philo_data->sim_stop_mutex);
		pthread_mutex_unlock(&philo->philo_data->log_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->philo_data->sim_stop_mutex);
	if (status == HELD_FORK)
		write_message(philo, "has taken a fork");
	else if (status == EATING)
		write_message(philo, "is eating");
	else if (status == THINKING)
		write_message(philo, "is thinking");
	else if (status == SLEEPING)
		write_message(philo, "is sleeping");
	else if (status == DEAD)
		write_message(philo, "died");
	pthread_mutex_unlock(&philo->philo_data->log_mutex);
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
