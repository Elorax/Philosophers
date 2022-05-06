/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   true_steps1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/06 16:12:04 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	pensage(t_philo *philo, struct timeval *time)
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
	printf("%d %d is thinking\n", ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->mut_end);
	if (philo->eaten > 0 && philo->nb_philo % 2)
		ft_msleep(philo->time_to_eat * 2 - philo->time_to_sleep, philo);
	return (1);
}

int	attendage(t_philo *philo)
{
	if (philo->nb_philo % 2)
	{
		if (philo->eaten == 0)
			ft_msleep(philo->time_to_eat * (philo->nb % 3), philo);
	}
	else
		if (philo->nb % 2 && philo->eaten == 0)
			ft_msleep(philo->time_to_eat, philo);
	return (1);
}

int	prendage_fork_one(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->fork[philo->nb - 1].mut);
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (1);
	}
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d %d has taken a fork\n",
		ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->mut_end);
	return (1);
}

int	prendage_fork_two(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->fork[philo->nb % philo->nb_philo].mut);
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (1);
	}
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d %d has taken a fork\n",
		ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->mut_end);
	return (1);
}

int	mangeage(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (1);
	}
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(philo->tv, NULL);
	printf("%d %d is eating\n", ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->mut_end);
	philo->eaten++;
	ft_msleep(philo->time_to_eat, philo);
	return (1);
}
