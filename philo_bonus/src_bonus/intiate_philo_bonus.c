/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intiate_philo_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 05:52:22 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/05 08:42:30 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

static int	allocation(long int n, char **res, int base)
{
	int			i;

	i = 1;
	n = n;
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	else if (n == 0)
		i++;
	while (n > 0)
	{
		n /= base;
		i++;
	}
	*res = (char *)malloc(sizeof(char) * i);
	return (i - 1);
}

char	*ft_itoa(int n)
{
	char		*res;
	int			i;
	long int	temp;

	i = allocation((int)n, &res, 10);
	if (res == NULL)
		return (NULL);
	temp = n;
	if (n < 0)
	{
		temp *= -1;
		res[0] = '-';
	}
	res[i--] = '\0';
	if (temp == 0)
		res[i] = '0';
	while (temp > 0)
	{
		res[i--] = (temp % 10) + '0';
		temp /= 10;
	}
	return (res);
}

void	creat_threads(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	while (i < philo_data->philos_count)
	{
		process = philo_data->process[i];
		process->id = fork();
		if (!process->id)
			routine(process);
		i++;
	}
}

void	create_processes(t_philo *philo_data)
{
	int				i;
	t_philo_process	*process;

	i = 0;
	philo_data->start_time = get_time_in_ms() + (philo_data->philos_count * 20);
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, i, 9);
		process = philo_data->process[i];
		process->meal_sem_name = ft_itoa(process->id);
		sem_unlink(process->meal_sem_name);
		process->meal_sem = sem_open(process->meal_sem_name, O_CREAT, 0644, 1);
		free(process->meal_sem_name);
		process->philo_num = i;
		process->philo_data = philo_data;
		process->state = EATING;
		process->last_meal = philo_data->start_time;
		process->meals = 0;
		i++;
	}
	creat_threads(philo_data);
}

void	intiate_philo_seph(t_philo *philo_data)
{
	int	can_eat;

	sem_unlink("log");
	sem_unlink("sim_stop");
	sem_unlink("fork");
	sem_unlink("can_eat");
	sem_unlink("sim_already_stopped");
	can_eat = philo_data->philos_count / 2 + philo_data->philos_count % 2;
	philo_data->can_eat = sem_open("can_eat", O_CREAT, 0644, can_eat);
	philo_data->sim_already_stopped = sem_open("sim_already_stopped",
			O_CREAT, 0644, 1);
	philo_data->log_sem = sem_open("log", O_CREAT, 0644, 1);
	philo_data->sim_stop_sem = sem_open("sim_stop", O_CREAT, 0644, 0);
	philo_data->fork = sem_open("fork", O_CREAT, 0644,
			philo_data->philos_count);
}

void	get_values(t_philo *philo_data, char *argv[], int argc)
{
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		philo_data->times_must_eat = ft_atoi(argv[5]);
		philo_data->limited_meals = 1;
	}
	else
	{
		philo_data->times_must_eat = 0;
		philo_data->limited_meals = 0;
	}
}

t_philo	*assign_philo(char *argv[], int argc)
{
	t_philo			*philo_data;

	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		philo_error_handling(NULL, 0, 2);
	get_values(philo_data, argv, argc);
	philo_data->process = NULL;
	philo_data->process = malloc(sizeof(t_philo_process *)
			* philo_data->philos_count);
	if (!philo_data->process)
		philo_error_handling(philo_data, 0, 3);
	intiate_philo_seph(philo_data);
	philo_data->sim_stop = 0;
	return (philo_data);
}
