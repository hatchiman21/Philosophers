/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/14 13:21:56 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHOLO_H

# include "libft/libft.h"
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define THINKING 0
# define EATING 1
# define SLEEPING 2

typedef struct s_utinsels
{
	int				is_used;
	pthread_mutex_t	mutex;
}					t_utinsels;

typedef struct s_philo_process
{
	pthread_t		thread;
	struct timeval	last_meal;
	struct timeval	start_time;
	struct timeval	current_time;
	int				state;
	int				philo_num;
	struct s_philo	*philo_data;
}					t_philo_process;

typedef struct s_philo
{
	int				philos_count;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				death;
	t_philo_process	**process;
	t_utinsels		**fork;
}					t_philo;

#endif
