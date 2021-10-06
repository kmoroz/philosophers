/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 14:23:20 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/06 16:38:41 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include "philo.h"

void	grab_fork(t_philo *philo, t_table *table)
{
	if (philo->left_fork == philo->right_fork)
	{
		if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
	}
	else if (philo->philo_id % 2 == 0)
	{
		if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		if (pthread_mutex_lock(&table->fork[philo->left_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
	}
	else
	{
		if (pthread_mutex_lock(&table->fork[philo->left_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
	}
}

void	eat(t_philo *philo, t_table *table)
{
	grab_fork(philo, table);
	if (philo->left_fork == philo->right_fork)
		pthread_mutex_unlock(&table->fork[philo->right_fork]);
	else
	{
		philo->recent_meal = get_current_time();
		if (philo->state == ALIVE)
		{
			printer(*philo, "is eating", "🍝");
			philo->meal_size--;
			spend_time(get_current_time(), philo->eat_time);
		}
		pthread_mutex_unlock(&table->fork[philo->left_fork]);
		pthread_mutex_unlock(&table->fork[philo->right_fork]);
	}
}

void	go_to_bed(t_philo *philo, int sleep_time)
{
	printer(*philo, "is sleeping", "💤");
	spend_time(get_current_time(), sleep_time);
}

void	*loop(void *arg)
{
	t_philo		*philo;
	t_table		*table;

	philo = (t_philo *)arg;
	table = philo->table;
	while (philo->state == ALIVE)
	{
		eat(philo, table);
		if (!philo->meal_size || philo->right_fork == philo->left_fork)
			return (NULL);
		go_to_bed(philo, philo->sleep_time);
		printer(*philo, "is thinking", "💭");
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_settings	settings;
	int			i;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		initialise(&settings, argv);
		if (settings.philo_size)
		{
			while (i < settings.philo_size)
			{
				pthread_create(&settings.philo[i].thread,
					NULL, loop, &settings.philo[i]);
				spend_time(get_current_time(), 2);
				i++;
			}
			pthread_create(&settings.checker, NULL, checker, &settings);
			while (i)
			{
				i--;
				pthread_join(settings.philo[i].thread, NULL);
			}
			pthread_join(settings.checker, NULL);
			free_everything(&settings);
		}
	}
}
