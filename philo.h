/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/27 07:07:41 by aatieh           ###   ########.fr       */
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
# define RIGHT 1
# define LEFT 2

typedef struct s_philo_process
{
	pthread_t		thread;
	long			last_meal;
	long			time;
	int				state;
	int				philo_num;
	int				is_dead;
	int				is_killed;
	struct s_philo	*philo_data;
}					t_philo_process;

typedef struct s_philo
{
	int				philos_count;
	unsigned int	t_to_die;
	unsigned int	t_to_eat;
	unsigned int	t_to_sleep;
	unsigned int	death;
	long			start_time;
	t_philo_process	**process;
	pthread_mutex_t	*fork;
	pthread_mutex_t	log_mutex;
}					t_philo;

long	ft_atoi(const char *str);
long	get_time_in_ms(void);
int		ft_isdigit(int c);
int		will_starve(void);
void	philo_error_handling(t_philo *phil, int error);
void	check_input(char *argv[], int argc);
int		check_starvation(t_philo_process *process,
			int holding_forks, int next_fork);

void	kill_the_rest(t_philo *philos);
void	*philo_life(void *arg);

#endif
