/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:18:48 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/27 07:07:32 by aatieh           ###   ########.fr       */
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

int	will_starve(void)
{
	// long	time_to_die;
	// long	time_to_wait;

	// time_to_die = process->last_meal + process->philo_data->t_to_die;
	// time_to_wait = get_time_in_ms() + process->philo_data->t_to_sleep;
	// if (time_to_die < time_to_wait)
		return (0);
	// usleep(time_to_die - time_to_wait);
	// pthread_mutex_lock(&process->philo_data->log_mutex);
	// if (!process->is_killed && !process->is_dead)
	// 	printf("%lu: Philosopher %d died\n", get_time_in_ms() - process->philo_data->start_time, process->philo_num + 1);
	// usleep(2000);
	// pthread_mutex_unlock(&process->philo_data->log_mutex);
	// process->philo_data->death = 1;
	// process->is_dead = 1;
	return (1);
}

int	check_starvation(t_philo_process *process, int holding_forks, int next_fork)
{
	unsigned long	time_diff;
	int				fork;

	fork = process->philo_num;
	time_diff = get_time_in_ms() - process->last_meal;
	if (time_diff >= process->philo_data->t_to_die)
	{
		pthread_mutex_lock(&process->philo_data->log_mutex);
		if (!process->is_killed && !process->is_dead)
			printf("%lu: Philosopher %d died\n", get_time_in_ms()
				- process->philo_data->start_time, process->philo_num + 1);
		usleep(2000);
		if (holding_forks)
		{
			pthread_mutex_unlock(&process->philo_data->fork[fork]);
			pthread_mutex_unlock(&process->philo_data->fork[next_fork]);
		}
		pthread_mutex_unlock(&process->philo_data->log_mutex);
		process->philo_data->death = 1;
		process->is_dead = 1;
		return (1);
	}
	return (0);
}

void	philo_error_handling(t_philo *phil, int error)
{
	int	i;

	i = 0;
	if (error == 1)
		write(2, "wrong input format\n", 19);
	else if (error == 2)
		write(2, "main philo malloc failed\n", 14);
	else if (error >= 3)
	{
		while (i < phil->philos_count && phil->process[i])
			free(phil->process[i++]);
		free(phil->process);
		if (error == 3)
			write(2, "pthread process malloc failed\n", 15);
		else if (error == 4)
		{
			i = 0;
			write(2, "pthread fork malloc failed\n", 22);
			while (i < phil->philos_count)
				pthread_mutex_destroy(&phil->fork[i++]);
			free(phil->fork);
		}
		free(phil);
	}
	exit(error);
}

void	check_input(char *argv[], int argc)
{
	int		i;
	int		j;

	if (argc != 5)
	{
		write(2, "wrong number of inputs\n", 24);
		exit (1);
	}
	i = 0;
	while (i++ < argc - 1)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			philo_error_handling(NULL, 1);
	}
}
