/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/06 21:58:28 by aatieh           ###   ########.fr       */
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
	sem_close(philos->enugh_meals);
	sem_unlink("enugh_meals");
	sem_unlink("meal");
	sem_unlink("sim_already_stopped");
	sem_unlink("log");
	sem_unlink("sim_stop");
	sem_unlink("fork");
	sem_unlink("can_eat");
	free(philos->process);
	free(philos);
}

void	*moniter_meals_eaten(void *arg)
{
	t_philo	*phlio_data;
	int		i;

	phlio_data = (t_philo *)arg;
	i = 0;
	while (i < phlio_data->philos_count)
	{
		sem_wait(phlio_data->enugh_meals);
		i++;
	}
	sem_wait(phlio_data->log_sem);
	sem_post(phlio_data->sim_stop_sem);
	sem_wait(phlio_data->sim_already_stopped);
	sem_post(phlio_data->log_sem);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_philo		*philo_data;
	pthread_t	monitor;

	check_input(argv, argc);
	philo_data = assign_philo_data(argv, argc);
	initiate_philo_sem(philo_data);
	allocate_philos(philo_data);
	creat_processes(philo_data);
	while (philo_data->start_time > get_time_in_ms())
		continue ;
	if (argc == 6)
	{
		if (pthread_create(&monitor, NULL, moniter_meals_eaten, philo_data))
			philo_error_handling(philo_data, 0, 7);
	}
	sem_wait(philo_data->sim_stop_sem);
	if (argc == 6)
		pthread_detach(monitor);
	stop_simulation(philo_data);
	return (0);
}
