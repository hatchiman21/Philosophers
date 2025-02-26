/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/26 05:30:11 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*check_entered_values(char *argv[], int argc)
{
	int	num;

	num = ft_atoi(argv[1]);
	if (num > 200 || num == 0)
		return (philo_error_handling(NULL, 0, 1));
	num = ft_atoi(argv[2]);
	if (num < 60 || num == 0)
		return (philo_error_handling(NULL, 0, 1));
	num = ft_atoi(argv[3]);
	if (num < 60 || num == 0)
		return (philo_error_handling(NULL, 0, 1));
	num = ft_atoi(argv[4]);
	if (num < 60 || num == 0)
		return (philo_error_handling(NULL, 0, 1));
	if (argc == 6 && ft_atoi(argv[5]) == 0)
		return (NULL);
	return (*argv);
}

void	*check_input(char *argv[], int argc)
{
	int		i;
	int		j;

	if (argc < 5)
	{
		write(2, "not enough arguments\n", 22);
		return (NULL);
	}
	else if (argc > 6)
	{
		write(2, "too many arguments\n", 20);
		return (NULL);
	}
	i = 0;
	while (i++ < argc - 1)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			return (philo_error_handling(NULL, 0, 1));
	}
	if (!check_entered_values(argv, argc))
		return (NULL);
	return (*argv);
}

void	stop_simulation(t_philo *philos)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philos->sim_stop_mutex);
	philos->sim_stop = 1;
	pthread_mutex_unlock(&philos->sim_stop_mutex);
	while (i < philos->philos_count)
	{
		pthread_join(philos->process[i]->thread, NULL);
		free(philos->process[i++]);
	}
	i = 0;
	while (i < philos->philos_count)
		pthread_mutex_destroy(&philos->fork[i++].mutex);
	pthread_mutex_destroy(&philos->log_mutex);
	pthread_mutex_destroy(&philos->sim_stop_mutex);
	pthread_mutex_destroy(&philos->meal_mutex);
	free(philos->process);
	free(philos->fork);
	free(philos);
}

void	main_thread_loop(t_philo *philo_data, int i, int eaten_enough)
{
	while (1)
	{
		if (philo_data->limited_meals)
			eaten_enough = 1;
		else
			eaten_enough = 0;
		i = 0;
		while (i < philo_data->philos_count)
		{
			if (check_starvation_and_meals(philo_data->process[i],
					philo_data, get_time_in_ms(), &eaten_enough))
			{
				write_status(philo_data->process[i], philo_data, DEAD);
				stop_simulation(philo_data);
				return ;
			}
			i++;
		}
		if (eaten_enough)
		{
			stop_simulation(philo_data);
			return ;
		}
	}
}

int	main(int argc, char *argv[])
{
	t_philo	*philo_data;

	if (!check_input(argv, argc))
		return (1);
	philo_data = assign_philo(argv, argc);
	if (!philo_data)
		return (1);
	if (!make_threads(philo_data))
		return (1);
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	main_thread_loop(philo_data, 0, 0);
	return (0);
}
