/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/01 17:40:17 by aatieh           ###   ########.fr       */
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
# include <semaphore.h>

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
	int				sim_stop;
	long			start_time;
	sem_t			fork;
	t_philo_process	**process;
	sem_t			sim_stop_mutex;
	sem_t			meal_mutex;
	sem_t			log_mutex;
}					t_philo;

int		starvation_sleeping_check(t_philo_process *process);
long	ft_atoi(const char *str);
long	get_time_in_ms(void);
int		ft_isdigit(int c);
void	philo_error_handling(t_philo *philo, int num, int error);
void	check_input(char *argv[], int argc);
void	forks_lock(t_philo_process *process, int fork1, int fork2);
int		my_usleep(t_philo_process *process, int time);
void	get_fork_num(t_philo_process *process, int *fork1, int *fork2);
int		check_starvation_inbetween(t_philo_process *process, long time_now);
void	write_status(t_philo_process *philo, int status);
void	kill_the_rest(t_philo *philos);
void	*philo_life(void *arg);

#endif
