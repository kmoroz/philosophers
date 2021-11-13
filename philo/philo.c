/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 14:23:20 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/13 12:01:49 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include "philo.h"

void	*loop(void *arg)
{
	t_philo		*philo;
	t_table		*table;

	philo = (t_philo *)arg;
	table = philo->table;
	while ("The prophecy is true")
	{
		pthread_mutex_lock(philo->pronounce_dead);
		if (philo->state == DEAD)
		{
			pthread_mutex_unlock(philo->pronounce_dead);
			break ;
		}
		pthread_mutex_unlock(philo->pronounce_dead);
		eat(philo, table);
		if (!philo->meal_size || philo->right_fork == philo->left_fork)
			return (NULL);
		go_to_bed(philo, philo->sleep_time);
		printer(*philo, "is thinking", "💭");
	}
	return (NULL);
}

void	create_threads(t_settings *settings)
{
	int			i;
	pthread_t	*thread;
	t_philo		*philo;
	pthread_t	*checker_thread;

	i = 0;
	checker_thread = &settings->checker;
	while (i < settings->philo_size)
	{
		philo = &settings->philo[i];
		thread = &settings->philo[i].thread;
		pthread_create(thread, NULL, loop, philo);
		i++;
	}
	pthread_create(checker_thread, NULL, checker, settings);
}

int	main(int argc, char **argv)
{
	t_settings	settings;
	int			i;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		if (initialise(&settings, argv) == OK)
		{
			create_threads(&settings);
			while (i < settings.philo_size)
			{
				pthread_join(settings.philo[i].thread, NULL);
				i++;
			}
			pthread_join(settings.checker, NULL);
			free_everything(&settings);
		}
	}
	else
		print_user_prompt(NOT_ENOUGH_ARGS);
}
