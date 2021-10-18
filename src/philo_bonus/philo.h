/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/14 17:41:31 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/18 14:16:11 by ksmorozo      ########   odam.nl         */
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

/*
** Status codes
*/

# define LOCKED 0
# define ERROR -1
# define OK 8

/*
** Philo states
*/

# define DEAD 6
# define ALIVE 7

# include <semaphore.h>
# include <pthread.h>

// typedef struct s_table
// {
// 	sem_t	fork;
// }				t_table;

typedef struct s_philo
{
	int				philo_id;
	int				pid;
	int				state;
	unsigned long	birth_time;
	unsigned long	sleep_time;
	unsigned long	eat_time;
	unsigned long	die_time;
	unsigned long	recent_meal;
	int				meal_size;
	sem_t			forks;
}				t_philo;

typedef struct s_settings
{
	int				philo_size;
	unsigned long	die_time;
	unsigned long	eat_time;
	unsigned long	sleep_time;
	int				meal_size;
	unsigned long	start_time;
	pthread_t		checker;
	pthread_t		killer;
	t_philo			*philo;
	sem_t			forks;
}				t_settings;

#endif
