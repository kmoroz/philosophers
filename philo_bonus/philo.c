/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/14 17:29:10 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/11 10:45:38 by ksmorozo      ########   odam.nl         */
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

void	close_semaphore(t_settings *settings)
{
	sem_close(settings->forks);
	sem_close(settings->state);
	sem_close(settings->pronounce_dead);
	sem_unlink("forks");
	sem_unlink("state");
	sem_unlink("pronounce dead");
}

void	start_children(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->philo_size)
	{
		settings->philo[i].pid = fork();
		if (i > 0)
			spend_time(get_current_time(), 5);
		if (settings->philo[i].pid == 0)
		{
			pthread_create(&settings->checker[i], NULL,
				checker, &settings->philo[i]);
			loop(&settings->philo[i]);
		}
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
		if (initialise(&settings, argv) == OK)
		{
			start_children(&settings);
			while (i < settings.philo_size)
			{
				pthread_join(settings.checker[i], NULL);
				i++;
			}
			kill_process(&settings);
			close_semaphore(&settings);
			free_everything(&settings);
		}
	}
	else
		print_user_prompt(NOT_ENOUGH_ARGS);
}
