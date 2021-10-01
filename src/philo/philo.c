/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 14:23:20 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/01 15:45:52 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
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

unsigned long	get_current_time(void)
{
	struct timeval	tv;
	unsigned long	result;

	gettimeofday(&tv, NULL);
	result = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (result);
}

unsigned long	timer(unsigned long birth_time)
{
	struct timeval	tv;
	unsigned long	result;

	gettimeofday(&tv, NULL);
	result = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - birth_time;
	return (result);
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
		settings->philo[i].state = ALIVE;
		settings->philo[i].left_fork = (i + 1) % settings->philo_size;
		settings->philo[i].right_fork = i;
		settings->philo[i].table = table;
		settings->philo[i].recent_meal = 0;
		settings->philo[i].sleep_time = settings->sleep_time;
		settings->philo[i].die_time = settings->die_time;
		settings->philo[i].eat_time = settings->eat_time;
		settings->philo[i].birth_time = settings->start_time;
		i++;
	}
}

void	initialise(t_settings *settings, char **argv)
{
	settings->start_time = get_current_time();
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

int	dead_or_alive(t_philo *philo)
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

void	printer(t_philo philo, char *str, char *emoji)
{
	if (philo.state == ALIVE || str == "died")
		printf("%-5lu Philosopher %-5d %-20s %s\n",
			timer(philo.birth_time), philo.philo_id, str, emoji);
}

void	*check_state(void *arg)
{
	t_settings	*settings;
	int			i;

	settings = (t_settings *)arg;
	i = 0;
	while ("The prophecy is true")
	{
		if (settings->philo_size == i)
			i = 0;
		if (dead_or_alive(&settings->philo[i]) == DEAD)
		{
			printer(settings->philo[i], "died", "⚰️");
			exit(0);
		}
		i++;
	}
}

void	grab_fork(t_philo *philo, t_table *table)
{
	if (philo->philo_id % 2 == 0)
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

void	spend_time(unsigned long current_time, unsigned long time)
{
    while ((get_current_time() - current_time) < time)
        usleep(100);
}

void	eat(t_philo *philo, t_table *table)
{
	grab_fork(philo, table);
	philo->recent_meal = get_current_time();
	if (philo->state == ALIVE)
	{
		printer(*philo, "is eating", "🍝");
		spend_time(get_current_time(), philo->eat_time);
	}
	pthread_mutex_unlock(&table->fork[philo->left_fork]);
	pthread_mutex_unlock(&table->fork[philo->right_fork]);
}

void	go_to_bed(t_philo *philo, int sleep_time)
{
	unsigned long	current_time;

	printer(*philo, "is sleeping", "💤");
	spend_time(get_current_time(), sleep_time);
}

void	think(t_philo *philo)
{
	printer(*philo, "is thinking", "💭");
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
		go_to_bed(philo, philo->sleep_time);
		think(philo);
	}
}

int	main(int argc, char **argv)
{
	t_settings	settings;
	int			i;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		initialise(&settings, argv);
		while (i < settings.philo_size)
		{
			pthread_create(&settings.philo[i].thread, NULL, loop, &settings.philo[i]);
			spend_time(get_current_time(), 2);
			i++;
		}
		pthread_create(&settings.checker, NULL, check_state, &settings);
		while (i)
		{
			i--;
			pthread_join(settings.philo[i].thread, NULL);
		}
		pthread_join(settings.checker, NULL);
	}
}
