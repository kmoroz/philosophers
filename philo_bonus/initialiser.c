/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialiser.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/04 14:17:25 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/23 13:10:18 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

static void	init_philo(t_settings *settings)
{
	int		i;

	i = 0;
	while (i < settings->philo_size)
	{
		settings->philo[i].philo_id = i + 1;
		settings->philo[i].philo_size = settings->philo_size;
		settings->philo[i].state = settings->state;
		settings->philo[i].recent_meal = 0;
		settings->philo[i].sleep_time = settings->sleep_time;
		settings->philo[i].die_time = settings->die_time;
		settings->philo[i].eat_time = settings->eat_time;
		settings->philo[i].birth_time = settings->start_time;
		settings->philo[i].meal_size = settings->meal_size;
		settings->philo[i].forks = settings->forks;
		settings->philo[i].state = settings->state;
		settings->philo[i].pronounce_dead = settings->pronounce_dead;
		settings->philo[i].meal_time = settings->meal_time;
		i++;
	}
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
	settings->checker = malloc(sizeof(pthread_t) * settings->philo_size);
	settings->philo = malloc(sizeof(t_philo) * settings->philo_size);
	if (!settings->checker || !settings->philo)
		return (ERROR);
	sem_unlink("forks");
	sem_unlink("state");
	sem_unlink("pronounce dead");
	settings->forks = sem_open("forks", O_CREAT,
			0600, settings->philo_size);
	settings->state = sem_open("state", O_CREAT, 0600, LOCKED);
	settings->pronounce_dead = sem_open("pronounce dead", O_CREAT, 0600, 1);
	settings->meal_time = sem_open("meal time", O_CREAT, 0600, 1);
	init_philo(settings);
	return (OK);
}
