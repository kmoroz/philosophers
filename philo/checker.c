/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checker.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/04 17:38:09 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/06 15:33:19 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	dead_or_alive(t_philo *philo)
{
	unsigned long	current_time;

	if (!philo->recent_meal)
		current_time = timer(philo->birth_time);
	else
		current_time = get_current_time();
	if (current_time >= philo->recent_meal + philo->die_time)
	{
		philo->state = DEAD;
		return (DEAD);
	}
	return (ALIVE);
}

static void	mark_everyone_dead(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->philo_size)
	{
		settings->philo[i].state = DEAD;
		i++;
	}
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
		if (!settings->philo[i].meal_size)
			return (NULL);
		if (dead_or_alive(&settings->philo[i]) == DEAD)
		{
			mark_everyone_dead(settings);
			printer(settings->philo[i], "died", "⚰️");
			return (NULL);
		}
		i++;
	}
}
