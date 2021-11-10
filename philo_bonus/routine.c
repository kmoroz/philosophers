/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/04 14:25:00 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/04 14:34:03 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	sem_wait(philo->forks);
	printer(*philo, "has taken a fork", "🍽️");
	sem_wait(philo->forks);
	printer(*philo, "has taken a fork", "🍽️");
	philo->recent_meal = get_current_time();
	printer(*philo, "is eating", "🍝");
	if (philo->meal_size)
		philo->meal_size--;
	spend_time(get_current_time(), philo->eat_time);
	sem_post(philo->forks);
	sem_post(philo->forks);
}

void	go_to_bed(t_philo *philo, int sleep_time)
{
	printer(*philo, "is sleeping", "💤");
	spend_time(get_current_time(), sleep_time);
}

void	*loop(t_philo *philo)
{
	while ("The prophecy is true")
	{
		eat(philo);
		go_to_bed(philo, philo->sleep_time);
		printer(*philo, "is thinking", "💭");
	}
	return (NULL);
}
