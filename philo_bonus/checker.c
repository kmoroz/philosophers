/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checker.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/04 14:21:09 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/11 10:48:56 by ksmorozo      ########   odam.nl         */
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
		return (DEAD);
	return (ALIVE);
}

void	*checker(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while ("The prophecy is true")
	{
		if (!philo->meal_size && philo->philo_id == philo->philo_size)
		{
			sem_wait(philo->pronounce_dead);
			sem_post(philo->state);
			return (NULL);
		}
		if (dead_or_alive(philo) == DEAD)
		{
			sem_wait(philo->pronounce_dead);
			printer(*philo, "died", "⚰️");
			sem_post(philo->state);
			return (NULL);
		}
	}
	return (NULL);
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
