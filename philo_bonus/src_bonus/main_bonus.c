/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/04 08:14:29 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	check_input(char *argv[], int argc)
{
	int		i;
	int		j;

	if (argc < 5)
	{
		write(2, "not enough arguments\n", 22);
		exit (1);
	}
	else if (argc > 6)
	{
		write(2, "too many arguments\n", 20);
		exit (1);
	}
	i = 0;
	while (i++ < argc - 1)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			philo_error_handling(NULL, 0, 1);
	}
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
					get_time_in_ms(), &eaten_enough))
			{
				write_status(philo_data->process[i], DEAD);
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

	check_input(argv, argc);
	philo_data = assign_philo(argv, argc);
	make_threads(philo_data);
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	main_thread_loop(philo_data, 0, 0);
	return (0);
}
