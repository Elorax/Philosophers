/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/06 15:39:11 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	init_mutex(t_data *data, struct timeval *time)
{
	int	i;

	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->mut_end, NULL);
	pthread_mutex_init(&data->mut_time, NULL);
	i = -1;
	while (++i < data->nb_philo)
	{
		data->fork[i].nb = 0;
		data->fork[i].mut = malloc(sizeof(pthread_mutex_t));
		if (data->fork[i].mut == NULL)
			data->crash = 1;
		pthread_mutex_init(data->fork[i].mut, NULL);
	}
	gettimeofday(time, NULL);
	return (0);
}

int	allocate(t_data *data, struct timeval *time)
{
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		exit((free(time), 1));
	data->fork = malloc(sizeof(t_fork) * data->nb_philo);
	if (!data->fork)
		exit((free(data->philo), free(time), EXIT_FAILURE));
	data->threads = calloc(sizeof(pthread_t), data->nb_philo);
	if (!data->threads)
		exit((free(data->philo), free(data->fork), free(time), EXIT_FAILURE));
	return (0);
}

int	verif_input(int ac, char **av, t_data *data)
{
	if (data->nb_philo < 1 || data->nb_philo > 200000 || data->time_to_die < 130
		|| data->time_to_eat < 60 || data->time_to_sleep < 60)
		exit((printf("Incorrect values.\n"), 0));
	if (!(is_arg_valid(av[1]) && is_arg_valid(av[2])
			&& is_arg_valid(av[3]) && is_arg_valid(av[4])
			&& ((ac == 6 && is_arg_valid(av[5])) || ac == 5)))
		exit((printf("Incorrect values.\n"), 0));
	if (data->nb_to_eat == 0)
		exit(0);
	return (0);
}

int	set_data(int ac, char **av, t_data *data)
{
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->end = 0;
	data->crash = 0;
	if (ac == 6)
		data->nb_to_eat = atoi(av[5]);
	else
		data->nb_to_eat = -1;
	return (0);
}

int	read_args(int ac)
{
	if (ac < 5 || ac > 6)
		exit((printf("Mauvais nombre d'arguments\n"), 0));
	return (0);
}
