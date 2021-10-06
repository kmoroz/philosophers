/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   freeing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/06 15:25:16 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/06 15:31:57 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

void	free_forks(t_settings *settings)
{
	pthread_mutex_t	*fork;
	int				i;

	fork = settings->philo[0].table->fork;
	while (i < settings->philo_size)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
	free(fork);
}

void	free_everything(t_settings *settings)
{
	free_forks(settings);
	free(settings->philo);
}
