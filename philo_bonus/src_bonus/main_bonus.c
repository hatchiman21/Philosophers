/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/05 20:31:34 by aatieh           ###   ########.fr       */
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
	while (i < philos->philos_count)
	{
		kill(philos->process[i]->id, SIGKILL);
		free(philos->process[i]);
		i++;
	}
	sem_close(philos->fork);
	sem_close(philos->log_sem);
	sem_close(philos->sim_stop_sem);
	sem_close(philos->can_eat);
	sem_close(philos->sim_already_stopped);
	sem_close(philos->meal_sem);
	sem_unlink("meal");
	sem_unlink("sim_already_stopped");
	sem_unlink("log");
	sem_unlink("sim_stop");
	sem_unlink("fork");
	sem_unlink("can_eat");
	free(philos->process);
	free(philos);
}

void	main_thread_loop(t_philo *philo_data, int i, int eaten_enough)
{
	while (1)
	{
		sem_wait(philo_data->sim_stop_sem);
		stop_simulation(philo_data);
		return ;
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
				stop_simulation(philo_data);
				return ;
			}
			i++;
		}
		if (eaten_enough)
		{
			sem_wait(philo_data->sim_stop_sem);
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
	create_processes(philo_data);
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	main_thread_loop(philo_data, 0, 0);
	return (0);
}
