/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checker.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/04 17:38:09 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/23 11:59:39 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	dead_or_alive(t_philo *philo)
{
	unsigned long	current_time;
	unsigned long	die_time;

	if (pthread_mutex_lock(philo->meal_time))
		return (ERROR);
	if (!philo->recent_meal)
		current_time = timer(philo->birth_time);
	else
		current_time = get_current_time();
	die_time = philo->recent_meal + philo->die_time;
	if (pthread_mutex_unlock(philo->meal_time))
		return (ERROR);
	if (current_time >= die_time)
	{
		if (pthread_mutex_lock(philo->pronounce_dead))
			return (ERROR);
		philo->state = DEAD;
		if (pthread_mutex_unlock(philo->pronounce_dead))
			return (ERROR);
		return (DEAD);
	}
	return (ALIVE);
}

static int	mark_everyone_dead(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->philo_size)
	{
		if (pthread_mutex_lock(settings->philo[i].pronounce_dead))
			return (ERROR);
		settings->philo[i].state = DEAD;
		if (pthread_mutex_unlock(settings->philo[i].pronounce_dead))
			return (ERROR);
		i++;
	}
	return (OK);
}

static int	check_meals(t_settings *settings, int i)
{
	if (pthread_mutex_lock(settings->philo[i].meal_count))
		return (0);
	if (!settings->philo[i].meal_size)
	{
		if (pthread_mutex_unlock(settings->philo[i].meal_count))
			return (0);
		return (0);
	}
	if (pthread_mutex_unlock(settings->philo[i].meal_count))
		return (0);
	return (OK);
}

void	*checker(void *arg)
{
	t_settings	*settings;
	int			i;

	settings = (t_settings *)arg;
	i = 0;
	while ("The prophecy is true")
	{
		if (settings->philo_size == i)
			i = 0;
		if (!check_meals(settings, i))
			return (NULL);
		if (dead_or_alive(&settings->philo[i]) == DEAD)
		{
			if (mark_everyone_dead(settings) == ERROR)
				return (NULL);
			printer(settings->philo[i], "died", "⚰️");
			return (NULL);
		}
		if (dead_or_alive(&settings->philo[i]) == ERROR)
			return (NULL);
		i++;
	}
}

int	check_input(t_settings *settings, char **argv)
{
	if (settings->philo_size <= 0 || settings->die_time <= 0
		|| settings->eat_time <= 0 || settings->sleep_time <= 0)
	{
		print_user_prompt(NEG_ZERO_INPUT);
		return (ERROR);
	}
	if (argv[MEAL_SIZE] && settings->meal_size <= 0)
	{
		print_user_prompt(NEG_ZERO_INPUT);
		return (ERROR);
	}
	return (OK);
}
