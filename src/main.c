/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:24:58 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/27 07:24:22 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	kill_the_rest(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->philos_count)
	{
		if (philos->process[i]->is_dead == 0)
			philos->process[i]->is_killed = 1;
		i++;
	}
	i = 0;
	while (i < philos->philos_count)
	{
		pthread_join(philos->process[i]->thread, NULL);
		free(philos->process[i++]);
	}
	i = 0;
	while (i < philos->philos_count)
		pthread_mutex_destroy(&philos->fork[i++]);
	pthread_mutex_destroy(&philos->log_mutex);
	free(philos->process);
	free(philos->fork);
	free(philos);
}

void	make_threads(t_philo *philo_data)
{
	int	i;

	pthread_mutex_init(&philo_data->log_mutex, NULL);
	i = 0;
	while (i < philo_data->philos_count)
		pthread_mutex_init(&philo_data->fork[i++], NULL);
	i = 0;
	while (i < philo_data->philos_count)
	{
		philo_data->process[i] = malloc(sizeof(t_philo_process));
		if (!philo_data->process[i])
			philo_error_handling(philo_data, 3);
		philo_data->process[i]->philo_num = i;
		philo_data->process[i]->philo_data = philo_data;
		philo_data->process[i]->is_dead = 0;
		philo_data->process[i]->state = EATING;
		pthread_create(&philo_data->process[i]->thread,
			NULL, &philo_life, philo_data->process[i]);
		i++;
	}
}

t_philo	*assign_philo(char *argv[])
{
	t_philo			*philo_data;
	unsigned long	size;

	philo_data = malloc(sizeof(t_philo));
	if (!philo_data)
		philo_error_handling(NULL, 2);
	philo_data->philos_count = ft_atoi(argv[1]);
	philo_data->t_to_die = ft_atoi(argv[2]);
	philo_data->t_to_eat = ft_atoi(argv[3]);
	philo_data->t_to_sleep = ft_atoi(argv[4]);
	philo_data->start_time = get_time_in_ms();
	size = sizeof(t_philo_process *) * philo_data->philos_count;
	philo_data->process = malloc(size);
	if (!philo_data->process)
		philo_error_handling(philo_data, 3);
	philo_data->death = 0;
	size = sizeof(pthread_mutex_t) * philo_data->philos_count;
	philo_data->fork = malloc(size);
	if (!philo_data->fork)
		philo_error_handling(philo_data, 4);
	return (philo_data);
}

int	check_waiting(int philo_num, int dir, int time_to_die, t_philo *philo_data)
{
	int		time_to_wait;
	int		next_philo;

	if (dir == RIGHT && philo_num == 0)
		next_philo = philo_data->philos_count - 1;
	else if (dir == RIGHT)
		next_philo = philo_num - 1;
	else if (dir == LEFT && philo_num == philo_data->philos_count - 1)
		next_philo = 0;
	else
		next_philo = philo_num + 1;
	if (get_time_in_ms() - philo_data->process[next_philo]->last_meal > philo_data->t_to_eat)
	{
		time_to_wait = philo_data->process[next_philo]->last_meal + philo_data->t_to_eat;
		if (time_to_wait >= time_to_die)
		(void)time_to_die;
			return (time_to_wait);
	}
	return (-1);
}

int	will_anyone_starve(t_philo *philo_data, int i)
{
	long	time_to_die;
	long	time_to_wait;

	if (philo_data->process[i]->state != EATING)
		return (0);
	if (get_time_in_ms() - philo_data->process[i]->last_meal > philo_data->t_to_eat)
		return (0);
	time_to_die = philo_data->process[i]->last_meal + philo_data->t_to_die;
	time_to_wait = check_waiting(i, RIGHT, time_to_die, philo_data);
	if (time_to_wait == -1)
		time_to_wait = check_waiting(i, LEFT, time_to_die, philo_data);
	if (time_to_wait == -1)
		return (0);
	pthread_mutex_lock(&philo_data->log_mutex);
	printf("%lu: Philosopher %d died\n", get_time_in_ms() - philo_data->start_time, i + 1);
	philo_data->process[i]->is_dead = 1;
	usleep(2000);
	pthread_mutex_unlock(&philo_data->log_mutex);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_philo	*philo_data;
	int		i;

	check_input(argv, argc);
	philo_data = assign_philo(argv);
	make_threads(philo_data);
	while (1)
	{
		i = 0;
		while (i < philo_data->philos_count)
		{
			if (will_anyone_starve(philo_data, i))
			{
				kill_the_rest(philo_data);
				return (0);
			}
			i++;
		}
		if (philo_data->death == 1)
			break ;
		// usleep(1000);
	}
	kill_the_rest(philo_data);
	return (0);
}
