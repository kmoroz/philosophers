/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/04 17:26:39 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/11/11 09:12:36 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
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

void	printer(t_philo philo, char *str, char *emoji)
{
	if (philo.state == ALIVE || !ft_strncmp(str, "died", 4))
		printf("%-5lu Philosopher %-5d %-20s %s\n",
			timer(philo.birth_time), philo.philo_id, str, emoji);
}

void	print_user_prompt(int code)
{
	if (code == NOT_ENOUGH_ARGS)
	{
		printf("\033[92mPlease input the following:\n");
		printf("./philo [👥 number_of_philosophers][⚰️ time_to_die]");
		printf("[🥫 time_to_eat][💤 time_to_sleep]\033[0m\n");
	}
	else if (code == NEG_ZERO_INPUT)
	{
		printf("\033[92m\"Please input values above zero\" - Plato\n");
	}
}
