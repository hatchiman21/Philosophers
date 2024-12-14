/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/14 17:22:02 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	kill_the_rest(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->philos_count)
	{
		pthread_detach(philos->process[i]->thread);
		write(2, "philo ", 7);
		int	m = i + 1 + '0';
		write(2, &m, 1);
		write(2, " has been killed\n", 18);
		free(philos->process[i++]);
	}
	i = 0;
	while (i < philos->philos_count)
	{
		pthread_mutex_destroy(&philos->fork[i]->mutex);
		free(philos->fork[i++]);
	}
	free(philos->process);
	free(philos->fork);
}

void	philo_error_handling(t_philo *phil, int error)
{
	if (error == 1)
		write(2, "wrong input format\n", 19);
	else if (error == 2)
		write(2, "malloc failed\n", 14);
	else if (error >= 3)
	{
		if (phil && phil->process)
			free(phil->process);
		if (phil && phil->fork)
			free(phil->fork);
		free(phil);
		write(2, "pthread malloc failed\n", 15);
	}
	exit(error);
}

void	*philo_life(void *arg)
{
	t_philo_process	*process;
	int				next_fork;

	process = (t_philo_process *)arg;
	gettimeofday(&process->last_meal, NULL);
	printf("%ld: philosopher %d has been born\n", process->last_meal.tv_usec, process->philo_num + 1);
	while (1)
	{
		gettimeofday(&process->start_time, NULL);
		if ((process->start_time.tv_sec - process->last_meal.tv_sec) * 1000000 + (process->start_time.tv_usec - process->last_meal.tv_usec) >= process->philo_data->t_to_die)
		{
			printf("%ld: Philosopher %d died\n", process->start_time.tv_usec, process->philo_num + 1);
			process->philo_data->death = 1;
			break ;
		}
		if (process->state == THINKING)
		{
			printf("%ld: Philosopher %d is thinking\n", process->start_time.tv_usec, process->philo_num + 1);
			gettimeofday(&process->current_time, NULL);
			process->state = EATING;
		}
		else if (process->state == EATING)
		{
			if (process->philo_num == 0)
				next_fork = process->philo_data->philos_count - 1;
			else
				next_fork = process->philo_num - 1;
			while(process->philo_data->fork[process->philo_num]->is_used || process->philo_data->fork[next_fork]->is_used)
			{
				printf("%ld: Philosopher %d is waiting for forks\n", process->current_time.tv_usec, process->philo_num + 1);
				usleep(1);
			}
			gettimeofday(&process->current_time, NULL);
			if ((process->current_time.tv_sec - process->last_meal.tv_sec) * 1000000 + (process->current_time.tv_usec - process->last_meal.tv_usec) >= process->philo_data->t_to_die)
			{
				printf("%ld: Philosopher %d died\n", process->current_time.tv_usec, process->philo_num + 1);
				break ;
			}
			process->philo_data->fork[process->philo_num]->is_used = 1;
			process->philo_data->fork[next_fork]->is_used = 1;
			pthread_mutex_lock(&process->philo_data->fork[process->philo_num]->mutex);
			pthread_mutex_lock(&process->philo_data->fork[next_fork]->mutex);
			printf("%ld: Philosopher %d is eating\n", process->start_time.tv_usec, process->philo_num + 1);
			usleep(process->philo_data->t_to_eat);
			process->philo_data->fork[process->philo_num]->is_used = 0;
			process->philo_data->fork[next_fork]->is_used = 0;
			gettimeofday(&process->last_meal, NULL);
			process->state = SLEEPING;
		}
		else if (process->state == SLEEPING)
		{
			printf("%ld: Philosopher %d is sleeping\n", process->start_time.tv_usec, process->philo_num + 1);
			usleep(process->philo_data->t_to_sleep);
			process->state = THINKING;
		}
	}
	return (NULL);
}

void	make_threads(t_philo *philo_data)
{
	int	i;

	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->fork[i] = malloc(sizeof(t_utinsels));
		if (!philo_data->fork[i])
			philo_error_handling(philo_data, 3);
		philo_data->fork[i]->is_used = 0;
		pthread_mutex_init(&philo_data->fork[i]->mutex, NULL);
		i++;
	}
	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i] || !philo_data->fork[i])
			philo_error_handling(philo_data, 3);
		philo_data->process[i]->philo_num = i;
		philo_data->process[i]->philo_data = philo_data;
		if (i % 3 == 0)
			philo_data->process[i]->state = EATING;
		else if (i % 3 == 1)
			philo_data->process[i]->state = THINKING;
		else
			philo_data->process[i]->state = SLEEPING;
		pthread_create(&philo_data->process[i]->thread, NULL, &philo_life, philo_data->process[i]);
		i++;
	}
}

t_philo	*assign_philo(char *argv[])
{
	int		i;
	int		j;
	t_philo	*philo_data;

	i = 0;
	while (i++ < 4)
	{
		j = 0;
		while (argv[i][j] && ft_isdigit(argv[i][j]))
			j++;
		if (argv[i][j])
			philo_error_handling(NULL, 1);
	}
	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		philo_error_handling(NULL, 2);
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	philo_data->process = malloc(sizeof(t_philo_process *) * philo_data->philos_count);
	philo_data->death = 0;
	philo_data->fork = malloc(sizeof(t_utinsels *) * philo_data->philos_count);
	if (!philo_data->process || !philo_data->fork)
		philo_error_handling(philo_data, 3);
	return (philo_data);
}

int	main(int argc, char *argv[])
{
	t_philo	*philo_data;

	if (argc != 5)
	{
		write(2, "wrong number of inputs\n", 24);
		return (1);
	}
	philo_data = assign_philo(argv);
	make_threads(philo_data);
	while (1)
	{
		if (philo_data->death)
			break ;
	}
	write(2, "let the killing start\n", 23);
	kill_the_rest(philo_data);
	return (0);
}
