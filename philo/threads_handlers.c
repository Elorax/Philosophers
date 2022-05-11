/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/11 19:48:41 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	destroy_data_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_mutex_destroy(data->fork[i].mut);
		free(data->fork[i].mut);
	}
	pthread_mutex_destroy(&data->print);
	free(data->philo);
	free(data->fork);
	free(data->threads);
	return (0);
}

int	join_threads(t_data *data)
{
	int	i;

	i = -1;
	pthread_join(data->death_thread, NULL);
	while (++i < data->nb_philo)
		pthread_join(data->threads[i], NULL);
	return (0);
}

int	create_threads(t_data *data)
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (++i < data->nb_philo)
	{
		gettimeofday(data->philo[i].tv, NULL);
		ret = pthread_create(&(data->threads[i]), NULL,
				ft_philo, &data->philo[i]);
		if (ret != 0)
		{
			data->crash = i;
			break ;
		}
	}
	ret = pthread_create(&data->death_thread, NULL, death_checker, data);
	if (ret != 0)
		data->crash = 1;
	return (0);
}

int	quit_crash(t_data *data)
{
	if (data->crash)
	{
		free(data->philo);
		free(data->fork);
		free(data->threads);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void	init_philo(t_data *data, struct timeval *time)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		data->philo[i].fork = data->fork;
		data->philo[i].nb_philo = data->nb_philo;
		data->philo[i].end = 0;
		data->philo[i].time_to_die = data->time_to_die;
		data->philo[i].time_to_eat = data->time_to_eat;
		data->philo[i].time_to_sleep = data->time_to_sleep;
		data->philo[i].print = &data->print;
		data->philo[i].mut_end = &data->mut_end;
		data->philo[i].mut_time = &data->mut_time;
		data->philo[i].tv = malloc(sizeof(struct timeval));
		if (data->philo[i].tv == NULL)
		{
			free_tv(data->philo, i);
			data->crash = 1;
		}
		data->philo[i].nb = i + 1;
		data->philo[i].eaten = 0;
		data->philo[i].to_eat = data->nb_to_eat;
		data->philo[i].tvi = time;
	}
}
