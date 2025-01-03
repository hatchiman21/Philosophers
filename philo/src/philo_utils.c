/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/01 17:40:17 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
	{
		pthread_mutex_lock(&philo->philo_data->sim_stop_mutex);
		philo->philo_data->sim_stop = 1;
		pthread_mutex_unlock(&philo->philo_data->sim_stop_mutex);
		write_message(philo, "died");
	}
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

// void	fork2_lock(t_philo_process *process, int fork1, int fork2)
// {
// 	process->philo_data->fork[fork1].is_used = 1;
// 	pthread_mutex_lock(&process->philo_data->log_mutex);
// 	if (!process->is_dead)
// 		printf("%lu: Philosopher %d has taken a fork\n", get_time_in_ms()
// 			- process->philo_data->start_time, process->philo_num + 1);
// 	pthread_mutex_unlock(&process->philo_data->log_mutex);
// 	pthread_mutex_unlock(&process->philo_data->fork[fork1].mutex);
// 	pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
// 	process->fork1_check = 1;
// 	while (!process->is_dead)
// 	{
// 		pthread_mutex_lock(&process->philo_data->fork[fork2].mutex);
// 		if (process->philo_data->fork[fork2].is_used == 0)
// 		{
// 			process->philo_data->fork[fork2].is_used = 1;
// 			printf("%lu: Philosopher %d has taken a fork\n", get_time_in_ms()
// 				- process->philo_data->start_time, process->philo_num + 1);
// 			pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
// 			break ;
// 		}
// 		else
// 		{
// 			pthread_mutex_unlock(&process->philo_data->fork[fork2].mutex);
// 			if (check_starvation_inbetween(process, get_time_in_ms()))
// 				break ;
// 		}
// 	}
// }

// void	forks_lock(t_philo_process *process, int fork1, int fork2)
// {
// 	pthread_mutex_lock(&process->philo_data->fork[fork1].mutex);
// 	write_status(process, HELD_FORK);
// 	pthread_mutex_lock(&process->philo_data->fork[fork2].mutex);
// 	write_status(process, HELD_FORK);
// }
