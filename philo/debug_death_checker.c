/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 21:32:20 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

void	check_finish(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		if (!data->philo[i].end)
		{
			break ;
		}
	}
	if (i == data->nb_philo)
		data->end = 1;
	i = -1;
	if (data->end == 1)
	{
		while (++i < data->nb_philo)
			data->philo[i].end = 1;
	}
	pthread_mutex_unlock(&data->mut_end);
	usleep(2000);
	pthread_mutex_lock(&data->mut_end);
}

void	print_death(t_data *data, struct timeval *time, int i)
{
	pthread_mutex_unlock(&data->mut_time);
	pthread_mutex_lock(&data->mut_end);
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->mut_time);
	printf("%d ms : Philosopher %d died : didn't eat for %d ms\n",
		ft_diff_time(data->philo[i].tvi, time), data->philo[i].nb,
		ft_diff_time(data->philo[i].tv, time));
	pthread_mutex_unlock(&data->mut_time);
	pthread_mutex_unlock(&data->print);
	data->end = 1;
}

void	check_deaths(t_data *data, struct timeval *time)
{
	int	i;

	i = 0;
	while (i < data->nb_philo && !data->end)
	{
		pthread_mutex_unlock(&data->mut_end);
		pthread_mutex_lock(&data->mut_time);
		if (data->philo[i].tv == NULL)
		{
			i++;
			pthread_mutex_unlock(&data->mut_time);
			pthread_mutex_lock(&data->mut_end);
			continue ;
		}
		if (ft_diff_time(data->philo[i].tv,
				time) > data->philo[i].time_to_die)
		{
			print_death(data, time, i);
			break ;
		}			
		pthread_mutex_unlock(&data->mut_time);
		i++;
		pthread_mutex_lock(&data->mut_end);
	}
}

void	*death_checker(void *param)
{
	struct timeval	*time;
	t_data			*data;

	data = (t_data *) param;
	time = malloc(sizeof(struct timeval));
	if (!time)
		return (NULL);
	pthread_mutex_lock(&data->mut_end);
	while (data->end == 0)
	{
		pthread_mutex_unlock(&data->mut_end);
		gettimeofday(time, NULL);
		pthread_mutex_lock(&data->mut_end);
		check_deaths(data, time);
		check_finish(data);
	}
	pthread_mutex_unlock(&data->mut_end);
	return (free(time), printf("Fin du PROGRAMME"), NULL);
}
