/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   freeing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/06 15:25:16 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/03 09:31:21 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

void	free_fork(int size, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
	free(fork);
}

void	free_everything(t_settings *settings)
{
	free_fork(settings->philo_size, settings->philo->table->fork);
	free(settings->philo->table);
	free(settings->philo);
}
