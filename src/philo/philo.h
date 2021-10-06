/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 15:16:39 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/06 15:26:14 by ksmorozo      ########   odam.nl         */
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

/*
** Philo states
*/

# define DEAD 6
# define ALIVE 7

# include <pthread.h>

typedef struct s_table
{
	pthread_mutex_t	*fork;
}				t_table;

typedef struct s_philo
{
	int				philo_id;
	int				state;
	int				left_fork;
	int				right_fork;
	unsigned long	birth_time;
	unsigned long	sleep_time;
	unsigned long	eat_time;
	unsigned long	die_time;
	unsigned long	recent_meal;
	int				meal_size;
	pthread_t		thread;
	t_table			*table;
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
	t_philo			*philo;
}				t_settings;

void			initialise(t_settings *settings, char **argv);
unsigned long	timer(unsigned long birth_time);
unsigned long	get_current_time(void);
void			spend_time(unsigned long current_time, unsigned long time);
int				ft_atoi(const char *str);
void			printer(t_philo philo, char *str, char *emoji);
void			*checker(void *arg);
void			free_everything(t_settings *settings);

#endif
