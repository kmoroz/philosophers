/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialiser.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/04 17:21:01 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/26 17:44:26 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

static int	init_forks(t_settings *settings, t_table *table)
{
	int	i;

	i = 0;
	table->fork = malloc(sizeof(pthread_mutex_t) * settings->philo_size);
	if (!table->fork)
		return (ERROR);
	while (i < settings->philo_size)
	{
		if (pthread_mutex_init(&table->fork[i], NULL) != 0)
			return (ERROR);
		i++;
	}
	return (OK);
}

static int	init_philo(t_settings *settings)
{
	int		i;
	t_table	*table;

	i = 0;
	table = malloc(sizeof(t_table));
	if (!table || init_forks(settings, table) == ERROR)
		return (ERROR);
	while (i < settings->philo_size)
	{
		settings->philo[i].philo_id = i + 1;
		settings->philo[i].state = ALIVE;
		settings->philo[i].left_fork = (i + 1) % settings->philo_size;
		settings->philo[i].right_fork = i;
		settings->philo[i].table = table;
		settings->philo[i].sleep_time = settings->sleep_time;
		settings->philo[i].die_time = settings->die_time;
		settings->philo[i].eat_time = settings->eat_time;
		settings->philo[i].birth_time = settings->start_time;
		settings->philo[i].meal_size = settings->meal_size;
		settings->philo[i].pronounce_dead = &settings->pronounce_dead;
		settings->philo[i].meal_count = &settings->meal_count;
		settings->philo[i].meal_time = &settings->meal_time;
		i++;
	}
	return (OK);
}

int	initialise(t_settings *settings, char **argv)
{
	settings->philo_size = ft_atoi(argv[PHILO_SIZE]);
	settings->start_time = get_current_time();
	settings->die_time = ft_atoi(argv[DIE_TIME]);
	settings->eat_time = ft_atoi(argv[EAT_TIME]);
	settings->sleep_time = ft_atoi(argv[SLEEP_TIME]);
	settings->meal_size = -1;
	if (argv[MEAL_SIZE])
		settings->meal_size = ft_atoi(argv[MEAL_SIZE]);
	if (check_input(settings, argv) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&settings->pronounce_dead, NULL) != 0
		|| pthread_mutex_init(&settings->meal_count, NULL) != 0
		|| pthread_mutex_init(&settings->meal_time, NULL) != 0)
		return (ERROR);
	settings->philo = malloc(sizeof(t_philo) * settings->philo_size);
	if (!settings->philo || init_philo(settings) == ERROR)
		return (ERROR);
	return (OK);
}
