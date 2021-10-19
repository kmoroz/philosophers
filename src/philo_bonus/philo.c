/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/14 17:29:10 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/19 16:25:35 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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

static int	ft_strncmp(const void *ptr1, const void *ptr2, size_t num)
{
	size_t			count;
	unsigned char	*buffer1;
	unsigned char	*buffer2;

	count = 0;
	buffer1 = (unsigned char *)ptr1;
	buffer2 = (unsigned char *)ptr2;
	while (count != num && (buffer1[count] != '\0' || buffer2[count] != '\0'))
	{
		if (buffer1[count] != buffer2[count])
			return (buffer1[count] - buffer2[count]);
		count++;
	}
	return (0);
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

void	printer(t_philo philo, char *str, char *emoji)
{
	if (philo.state == ALIVE || !ft_strncmp(str, "died", 4))
		printf("%-5lu Philosopher %-5d %-20s %s\n",
			timer(philo.birth_time), philo.philo_id, str, emoji);
}

void	spend_time(unsigned long current_time, unsigned long time)
{
	while ((get_current_time() - current_time) < time)
		usleep(100);
}

// static void	init_forks(t_settings *settings, t_table *table)
// {
// 	int	i;

// 	i = 0;
// 	table->fork = malloc(sizeof(pthread_mutex_t) * settings->philo_size);
// 	while (i < settings->philo_size)
// 	{
// 		sem_init(&table->fork[i], 1, settings->philo_size);
// 		i++;
// 	}
// }

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

void	kill_all(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->philo_size)
	{
		kill(settings->philo[i].pid, SIGKILL);
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
			kill_all(settings);
			exit(0);
		}
		i++;
	}
}

static void	init_philo(t_settings *settings)
{
	int		i;
	// t_table	table;

	i = 0;
	// init_forks(settings, &table);
	while (i < settings->philo_size)
	{
		settings->philo[i].philo_id = i + 1;
		settings->philo[i].state = ALIVE;
		settings->philo[i].recent_meal = 0;
		settings->philo[i].sleep_time = settings->sleep_time;
		settings->philo[i].die_time = settings->die_time;
		settings->philo[i].eat_time = settings->eat_time;
		settings->philo[i].birth_time = settings->start_time;
		settings->philo[i].meal_size = settings->meal_size;
		// settings->philo[i].table = &table;
		settings->philo[i].forks = settings->forks;
		i++;
	}
}

int	initialise(t_settings *settings, char **argv)
{
	settings->philo_size = ft_atoi(argv[PHILO_SIZE]);
	if (settings->philo_size)
	{
		settings->start_time = get_current_time();
		settings->die_time = ft_atoi(argv[DIE_TIME]);
		settings->eat_time = ft_atoi(argv[EAT_TIME]);
		settings->sleep_time = ft_atoi(argv[SLEEP_TIME]);
		settings->meal_size = -1;
		if (argv[MEAL_SIZE])
			settings->meal_size = ft_atoi(argv[MEAL_SIZE]);
		settings->checker = malloc(sizeof(pthread_t) * settings->philo_size);
		settings->philo = malloc(sizeof(t_philo) * settings->philo_size);
		sem_init(&settings->forks, 1, settings->philo_size);
		init_philo(settings);
		return (OK);
	}
	return (ERROR);
}

void	eat(t_philo *philo)
{
	sem_wait(&philo->forks);
	printer(*philo, "has taken a fork", "🍽️");
	sem_wait(&philo->forks);
	printer(*philo, "has taken a fork", "🍽️");
	philo->recent_meal = get_current_time();
	printer(*philo, "is eating", "🍝");
	philo->meal_size--;
	spend_time(get_current_time(), philo->eat_time);
	sem_post(&philo->forks);
	sem_post(&philo->forks);
}

void	go_to_bed(t_philo *philo, int sleep_time)
{
	printer(*philo, "is sleeping", "💤");
	spend_time(get_current_time(), sleep_time);
}

void*	loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->state == ALIVE)
	{
		eat(philo);
		go_to_bed(philo, philo->sleep_time);
		printer(*philo, "is thinking", "💭");
	}
	return (NULL);
}

void	*killer(void *arg)
{
	t_settings	*settings;
	int			i;

	i = 0;
	settings = (t_settings *)arg;
	while (i < settings->philo_size)
	{
		kill(settings->philo[i].pid, SIGKILL);
		i++;
	}
	return (NULL);
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
			settings.philo[i].pid = fork();
			spend_time(get_current_time(), 2);
			if (settings.philo[i].pid == 0)
			{
				pthread_create(&settings.checker[i], NULL, checker, &settings);
				loop(&settings.philo[i]);
			}
			i++;
		}
		//pthread_create(&settings.killer, NULL, killer, &settings);
		//pthread_join(settings.killer, NULL);
		//return (0);
		//pthread_join(settings.checker, NULL);
		i = 0;
		while (i < settings.philo_size)
		{
			pthread_join(settings.checker[i], NULL);
			i++;
		}
	}
}
