/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 14:23:20 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/22 13:48:39 by ksmorozo      ########   odam.nl         */
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
		if (pthread_create(thread, NULL, loop, philo))
			return ;
		spend_time(get_current_time(), 1);
		i++;
	}
	if (pthread_create(checker_thread, NULL, checker, settings))
		return ;
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
				if (pthread_join(settings.philo[i].thread, NULL))
					return (ERROR);
				i++;
			}
			if (pthread_join(settings.checker, NULL))
				return (ERROR);
			free_everything(&settings);
		}
	}
	else
		print_user_prompt(NOT_ENOUGH_ARGS);
}
