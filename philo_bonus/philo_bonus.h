/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/05 07:37:45 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/types.h>
# include <signal.h>

# define EATING 0
# define THINKING 1
# define SLEEPING 2
# define HELD_FORK 3
# define DEAD 4
# define RIGHT 1
# define LEFT 2

typedef struct s_philo_process
{
	int				id;
	char			*meal_sem_name;
	long			last_meal;
	int				state;
	int				philo_num;
	int				meals;
	sem_t			*meal_sem;
	struct s_philo	*philo_data;
}					t_philo_process;

typedef struct s_philo
{
	int				philos_count;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				times_must_eat;
	int				limited_meals;
	int				sim_stop;
	long			start_time;
	sem_t			*fork;
	sem_t			*sim_stop_sem;
	sem_t			*log_sem;
	sem_t			*can_eat;
	sem_t			*sim_already_stopped;
	t_philo_process	**process;
}					t_philo;

long	ft_atoi(const char *str);
int		ft_isdigit(int c);

long	get_time_in_ms(void);
int		my_usleep(t_philo_process *process, int time);
void	write_status(t_philo_process *philo, int status);

int		check_starvation(t_philo_process *process, long time_now);
int		check_starvation_and_meals(t_philo_process *process,
			t_philo *philo_data, long time_now, int *eaten_enough);

void	check_input(char *argv[], int argc);
void	philo_error_handling(t_philo *philo, int num, int error);
t_philo	*assign_philo(char *argv[], int argc);
void	create_processes(t_philo *philo_data);
void	routine(void *arg);

#endif