/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 21:29:23 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

void	ft_msleep(int msec, t_philo *philo)
{
	struct timeval	t1;
	struct timeval	t2;

	if (msec < 0)
		return ;
	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	while (ft_diff_time(&t1, &t2) < msec)
	{
		usleep (500);
		pthread_mutex_lock(philo->mut_end);
		if (philo->end)
		{
			pthread_mutex_unlock(philo->mut_end);
			return ;
		}
		pthread_mutex_unlock(philo->mut_end);
		gettimeofday(&t2, NULL);
	}
}

int	ft_diff_time(struct timeval *t1, struct timeval *t2)
{
	int	diff;

	if (!t1 || !t2)
		return (-1);
	diff = t2->tv_sec - t1->tv_sec;
	diff *= 1000;
	diff += (t2->tv_usec - t1->tv_usec) / 1000;
	return (diff);
}

void	free_tv(t_philo *philo, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(philo[i].tv);
		i++;
	}
}
