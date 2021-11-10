/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/04 17:28:10 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/10/04 17:34:16 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include "philo.h"

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

void	spend_time(unsigned long current_time, unsigned long time)
{
	while ((get_current_time() - current_time) < time)
		usleep(100);
}
