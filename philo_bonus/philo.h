/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/14 17:41:31 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/11 14:38:31 by ksmorozo      ########   odam.nl         */
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

/*
** User prompt codes
*/

# define NOT_ENOUGH_ARGS 1
# define NEG_ZERO_INPUT 2

# include <semaphore.h>
# include <pthread.h>

typedef struct s_philo
{
	int				philo_id;
	int				philo_size;
	int				pid;
	unsigned long	birth_time;
	long			sleep_time;
	long			eat_time;
	long			die_time;
	unsigned long	recent_meal;
	int				meal_size;
	sem_t			*forks;
	sem_t			*state;
	sem_t			*pronounce_dead;
}				t_philo;

typedef struct s_settings
{
	int				philo_size;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	int				meal_size;
	unsigned long	start_time;
	pthread_t		*checker;
	pthread_t		killer;
	t_philo			*philo;
	sem_t			*forks;
	sem_t			*state;
	sem_t			*pronounce_dead;
}				t_settings;

void			printer(t_philo philo, char *str, char *emoji);
int				ft_atoi(const char *str);
unsigned long	timer(unsigned long birth_time);
void			spend_time(unsigned long current_time, unsigned long time);
unsigned long	timer(unsigned long birth_time);
unsigned long	get_current_time(void);
int				initialise(t_settings *settings, char **argv);
void			*checker(void *arg);
void			*loop(t_philo *philo);
void			go_to_bed(t_philo *philo, int sleep_time);
void			eat(t_philo *philo);
void			free_everything(t_settings *settings);
void			print_user_prompt(int code);
int				check_input(t_settings *settings, char **argv);

#endif
