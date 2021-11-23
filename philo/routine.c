/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/20 16:17:53 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/23 11:42:52 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	grab_single_fork(t_philo *philo, t_table *table)
{
	if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
		printer(*philo, "has taken a fork", "🍽️");
	else
		return (ERROR);
	return (OK);
}

static int	grab_fork(t_philo *philo, t_table *table)
{
	if (philo->left_fork == philo->right_fork)
		return (grab_single_fork(philo, table));
	if (philo->philo_id % 2 == 0)
	{
		if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		else
			return (ERROR);
		if (pthread_mutex_lock(&table->fork[philo->left_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		else
			return (ERROR);
	}
	else
	{
		if (pthread_mutex_lock(&table->fork[philo->left_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		else
			return (ERROR);
		if (pthread_mutex_lock(&table->fork[philo->right_fork]) == LOCKED)
			printer(*philo, "has taken a fork", "🍽️");
		else
			return (ERROR);
	}
	return (OK);
}

int	eat(t_philo *philo, t_table *table)
{
	if (grab_fork(philo, table) == ERROR)
		return (ERROR);
	if (philo->left_fork == philo->right_fork)
		pthread_mutex_unlock(&table->fork[philo->right_fork]);
	else
	{
		if (pthread_mutex_lock(philo->meal_time))
			return (ERROR);
		philo->recent_meal = get_current_time();
		if (pthread_mutex_unlock(philo->meal_time))
			return (ERROR);
		printer(*philo, "is eating", "🍝");
		if (pthread_mutex_lock(philo->meal_count))
			return (ERROR);
		philo->meal_size--;
		if (pthread_mutex_unlock(philo->meal_count))
			return (ERROR);
		spend_time(get_current_time(), philo->eat_time);
		if (pthread_mutex_unlock(&table->fork[philo->left_fork])
			|| pthread_mutex_unlock(&table->fork[philo->right_fork]))
			return (ERROR);
	}
	return (OK);
}

void	go_to_bed(t_philo *philo, int sleep_time)
{
	printer(*philo, "is sleeping", "💤");
	spend_time(get_current_time(), sleep_time);
}
