/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 14:23:20 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/09/25 16:51:43 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

int	ft_atoi(const char *str)
{
	int	count;
	int	strtonum;
	int	isnegativenum;

	count = 0;
	strtonum = 0;
	isnegativenum = 1;
	while (str[count] == ' ' || str[count] == '\n' || str[count] == '\f'
		|| str[count] == '\r' || str[count] == '\t' || str[count] == '\v')
		count++;
	if (str[count] == '-' || str[count] == '+')
	{
		if (str[count] == '-')
			isnegativenum = -1;
		count++;
	}
	while (str[count] >= '0' && str[count] <= '9')
	{
		strtonum = 10 * strtonum + (str[count] - '0');
		count++;
	}
	if (isnegativenum == -1)
		strtonum = strtonum * -1;
	return (strtonum);
}

void	init_table(t_settings *settings, t_table *table)
{
	int	i;

	i = 0;
	table->fork = malloc(sizeof(pthread_mutex_t) * settings->philo_size);
	while (i < settings->philo_size)
	{
		pthread_mutex_init(&table->fork[i], NULL);
		i++;
	}
}

void	init_philo(t_settings *settings)
{
	int		i;
	t_table	*table;

	i = 0;
	table = malloc(sizeof(t_table));
	init_table(settings, table);
	while (i < settings->philo_size)
	{
		settings->philo[i].philo_id = i;
		settings->philo[i].left_fork = (i + 1) % settings->philo_size;
		settings->philo[i].right_fork = i;
		settings->philo[i].table = table;
		i++;
	}
}

void	initialise(t_settings *settings, char **argv)
{
	settings->philo_size = ft_atoi(argv[PHILO_SIZE]);
	settings->die_time = ft_atoi(argv[DIE_TIME]);
	settings->eat_time = ft_atoi(argv[EAT_TIME]);
	settings->sleep_time = ft_atoi(argv[SLEEP_TIME]);
	settings->meal_size = 0;
	if (argv[MEAL_SIZE])
		settings->meal_size = ft_atoi(argv[MEAL_SIZE]);
	settings->philo = malloc(sizeof(t_philo) * settings->philo_size);
	init_philo(settings);
}

void	*eat(void *arg)
{
	t_philo		*philo;
	t_table		*table;

	philo = (t_philo *)arg;
	table = philo->table;
	printf("Philosopher %d has taken a fork\n", philo->philo_id);
	pthread_mutex_lock(&table->fork[philo->right_fork]);
	pthread_mutex_lock(&table->fork[philo->left_fork]);
	printf("Philosopher %d is eating\n", philo->philo_id);
	pthread_mutex_unlock(&table->fork[philo->right_fork]);
	pthread_mutex_unlock(&table->fork[philo->left_fork]);
	printf("Philosopher %d has finished eating\n", philo->philo_id);
}

int	main(int argc, char **argv)
{
	t_settings	settings;

	if (argc == 5 || argc == 6)
	{
		initialise(&settings, argv);
		for (int i = 0; i < settings.philo_size; i++)
		{
			pthread_create(&settings.philo[i].thread, NULL, eat, &settings.philo[i]);
		}
		for (int i = 0; i < settings.philo_size; i++)
		{
			pthread_join(settings.philo[i].thread, NULL);
		}
		usleep(10000);
	}
}
