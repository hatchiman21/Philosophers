/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/26 03:01:52 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define EATING 0
# define THINKING 1
# define SLEEPING 2
# define HELD_FORK 3
# define DEAD 4
# define RIGHT 1
# define LEFT 2

typedef struct s_philo_process
{
	pthread_t		thread;
	long			last_meal;
	int				state;
	int				philo_num;
	int				meals;
	int				fork1;
	int				fork2;
	struct s_philo	*philo_data;
}					t_philo_process;

typedef struct s_eating_fork
{
	int				is_used;
	pthread_mutex_t	mutex;
}					t_eating_fork;

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
	t_eating_fork	*fork;
	t_philo_process	**process;
	pthread_mutex_t	sim_stop_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	log_mutex;
}					t_philo;

long	ft_atoi(const char *str);
int		ft_isdigit(int c);

long	get_time_in_ms(void);
int		my_usleep(t_philo *philo_data, int time);
void	write_status(t_philo_process *process, t_philo *philo_data, int status);

void	get_fork_num(t_philo_process *process, int *fork1, int *fork2);
void	grap_fork(t_philo_process *process, t_philo *philo_data, int fork);
void	let_go_of_fork(t_philo *philo_data, int fork);

int		check_starvation(t_philo_process *process,
			t_philo *philo_data, long time_now);
int		check_starvation_and_meals(t_philo_process *process,
			t_philo *philo_data, long time_now, int *meals);

void	*check_input(char *argv[], int argc);
void	*philo_error_handling(t_philo *philo, int num, int error);
t_philo	*assign_philo(char *argv[], int argc);
void	*make_threads(t_philo *philo_data);
void	*routine(void *arg);

#endif
