/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/14 17:29:10 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/28 13:38:51 by ksmorozo      ########   odam.nl         */
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
	printf("%-5lu Philosopher %-5d %-20s %s\n",
		timer(philo.birth_time), philo.philo_id, str, emoji);
}

void	spend_time(unsigned long current_time, unsigned long time)
{
	while ((get_current_time() - current_time) < time)
		usleep(100);
}

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
	while (1)
	{
		if (!philo->meal_size && philo->philo_id == philo->philo_size)
		{
			//printf("nom nom over\n");
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
		sem_unlink("forks");
		sem_unlink("state");
		sem_unlink("pronounce dead");
		settings->forks = sem_open("forks", O_CREAT, 0600, settings->philo_size);
		settings->state = sem_open("state", O_CREAT, 0600, LOCKED);
		settings->pronounce_dead = sem_open("pronounce dead", O_CREAT, 0600, 1);
		init_philo(settings);
		return (OK);
	}
	return (ERROR);
}

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

void*	loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->meal_size)
	{
		eat(philo);
		if (!philo->meal_size)
			return (NULL);
		go_to_bed(philo, philo->sleep_time);
		printer(*philo, "is thinking", "💭");
	}
	return (NULL);
}

void	kill_process(void *arg)
{
	t_settings	*settings;
	int			i;

	i = 0;
	settings = (t_settings *)arg;
	sem_wait(settings->state);
	while (i < settings->philo_size)
	{
		kill(settings->philo[i].pid, SIGKILL);
		i++;
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
			settings.philo[i].pid = fork();
			spend_time(get_current_time(), 1);
			if (settings.philo[i].pid == 0)
			{
				pthread_create(&settings.checker[i], NULL, checker, &settings.philo[i]);
				loop(&settings.philo[i]);
			}
			i++;
		}
		i = 0;
		while (i < settings.philo_size)
		{
			pthread_join(settings.checker[i], NULL);
			i++;
		}
		kill_process(&settings);
		sem_close(settings.forks);
		sem_close(settings.state);
		sem_close(settings.pronounce_dead);
		sem_unlink("forks");
		sem_unlink("state");
		sem_unlink("pronounce dead");
	}
}
