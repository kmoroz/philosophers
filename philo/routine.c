/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/20 16:17:53 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/20 16:18:34 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	grab_fork(t_philo *philo, t_table *table)
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
