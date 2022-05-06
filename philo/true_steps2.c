/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   true_steps2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/06 16:12:38 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	lachage_fork_one(t_philo *philo, struct timeval *time)
{
	(void) time;
	pthread_mutex_unlock(philo->fork[philo->nb - 1].mut);
	return (1);
}

int	lachage_fork_two(t_philo *philo, struct timeval *time)
{
	(void) time;
	pthread_mutex_unlock(philo->fork[philo->nb % philo->nb_philo].mut);
	return (1);
}

int	dormissage(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d %d is sleeping\n", ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->mut_end);
	if (philo->eaten == philo->to_eat)
	{
		pthread_mutex_lock(philo->mut_end);
		philo->end = 1;
		pthread_mutex_unlock(philo->mut_end);
	}
	ft_msleep(philo->time_to_sleep, philo);
	return (1);
}
