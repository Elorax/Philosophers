/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 21:17:37 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

void	boucle_philo(t_philo *philo, struct timeval *time)
{
	pthread_mutex_unlock(philo->mut_end);
	pensage(philo, time);
	attendage(philo);
	if (philo->nb % 2)
	{
		prendage_fork_one(philo, time);
		prendage_fork_two(philo, time);
	}
	else
	{
		prendage_fork_two(philo, time);
		prendage_fork_one(philo, time);
	}
	mangeage(philo, time);
	lachage_fork_one(philo, time);
	lachage_fork_two(philo, time);
	dormissage(philo, time);
	pthread_mutex_lock(philo->mut_end);
}

void	handle_solo_philo(t_philo *philo, struct timeval *time)
{
	pensage(philo, time);
	prendage_fork_one(philo, time);
	pthread_mutex_lock(philo->mut_end);
	while (philo->end == 0)
	{
		pthread_mutex_unlock(philo->mut_end);
		usleep(2000);
		pthread_mutex_lock(philo->mut_end);
	}
	pthread_mutex_unlock(philo->mut_end);
	lachage_fork_one(philo, time);
}

void	*ft_philo(void *param)
{
	struct timeval	*time;
	t_philo			*philo;

	philo = (t_philo *) param;
	time = malloc(sizeof(struct timeval));
	if (time == NULL)
		return (NULL);
	if (philo->nb_philo == 1)
		handle_solo_philo(philo, time);
	else
	{
		pthread_mutex_lock(philo->mut_end);
		while ((philo->to_eat == -1
				|| philo->eaten < philo->to_eat) && philo->end == 0)
			boucle_philo(philo, time);
		pthread_mutex_unlock(philo->mut_end);
	}
	time = (free(time), NULL);
	pthread_mutex_lock(philo->mut_time);
	philo->tv = (free(philo->tv), NULL);
	pthread_mutex_unlock(philo->mut_time);
	return (NULL);
}
