/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 15:16:39 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/09/25 15:07:15 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*
** Arguements' index
*/

# define PHILO_SIZE 1
# define DIE_TIME 2
# define EAT_TIME 3
# define SLEEP_TIME 4
# define MEAL_SIZE 5

# include <pthread.h>

/*
** Объявим структуру «философ», которая будет хранить имя философа и номера вилок, которые он может взять.
*/

typedef struct s_philo
{
	int			philo_id;
	int			left_fork;
	int			right_fork;
	pthread_t	thread;
}				t_philo;

/*
** Далее структура «стол», которая состоит их массива вилок. В качестве вилки будет выступать мьютекс. Блокировка мьютекса означает «взять вилку», а разблокировка «положить её обратно».
*/

typedef struct s_table
{
	pthread_mutex_t	*fork;
}				t_table;

typedef struct s_settings
{
	int		philo_size;
	int		die_time;
	int		eat_time;
	int		sleep_time;
	int		meal_size;
	t_philo	*philo;
	t_table	*table;
}				t_settings;

#endif
