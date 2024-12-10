/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:25:46 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/10 21:53:16 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHOLO_H

# include "libft/libft.h"

typedef struct s_philo_child
{
	int						child_num;
	struct s_philo_child	*next;
}							t_philo_child;

typedef struct s_philo
{
	int				philo_num;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	t_philo_child	*first_child;
}					t_philo;

#endif
