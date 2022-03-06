/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/03/06 17:22:02 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

typedef struct s_fork
{
	int				nb;
	pthread_mutex_t	*mut;
}	t_fork;

typedef struct s_philo
{
	int				nb;
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	struct timeval	*tv;
	struct timeval	*tvi;
	int				eaten;
	int				to_eat;
	int				state;
	int				end;
	pthread_mutex_t	*print;
	pthread_mutex_t	*mut_end;
	pthread_mutex_t	*mut_time;
	t_fork			*fork;
}	t_philo;

enum {thinking, eating, sleeping};

typedef struct s_data
{
	int				nb_philo;
	int				end;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	t_philo			*philo;
	t_fork			*fork;
	pthread_t		*threads;
	pthread_t		death_thread;
	pthread_mutex_t	print;
	pthread_mutex_t	mut_end;
	pthread_mutex_t	mut_nb_philo;
	pthread_mutex_t	mut_time;
}	t_data;

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

void	check_finish(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_mutex_lock(&data->mut_end);
		if (!data->philo[i].end)
		{
			pthread_mutex_unlock(&data->mut_end);
			break ;
		}
			pthread_mutex_unlock(&data->mut_end);
	}
	if (i == data->nb_philo)
		data->end = 1;
	i = -1;
	if (data->end == 1)
	{
		pthread_mutex_lock(&data->mut_end);
		while (++i < data->nb_philo)
			data->philo[i].end = 1;
		pthread_mutex_unlock(&data->mut_end);
	}
}

void	*death_checker(void *param)
{
	int				i;
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
		i = 0;
		gettimeofday(time, NULL);
		//pthread_mutex_lock(&data->print);
		pthread_mutex_lock(&data->mut_end);
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
			if (ft_diff_time(data->philo[i].tv, time) > data->philo[i].time_to_die)
			{
				pthread_mutex_unlock(&data->mut_time);
				pthread_mutex_lock(&data->print);
				pthread_mutex_lock(&data->mut_time);
				printf("%d ms : Philosopher %d died : didn't eat for %d ms\n", ft_diff_time(data->philo[i].tvi, time), data->philo[i].nb, ft_diff_time(data->philo[i].tv, time));
				pthread_mutex_unlock(&data->mut_time);
				pthread_mutex_unlock(&data->print);
				pthread_mutex_lock(&data->mut_end);
				data->end = 1;
				break ;
			}
			pthread_mutex_unlock(&data->mut_time);
			i++;
			pthread_mutex_lock(&data->mut_end);
		}
		pthread_mutex_unlock(&data->mut_end);
		check_finish(data);
		usleep(2000);
		pthread_mutex_lock(&data->mut_end);
	}
	pthread_mutex_unlock(&data->mut_end);
	return (free(time), printf("Fin du PROGRAMME"), NULL);
}

int	pensage(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d is thinking...\n",
		ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	return (1);
}

int	attendage(t_philo *philo)
{
	if (philo->nb % 2 && philo->eaten == 0)
		usleep(1000 * philo->time_to_eat);
	return (1);
}

int	prendage_fork_one(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->fork[philo->nb - 1].mut);
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->fork[philo->nb - 1].mut);
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d has taken fork %d...\n",
		ft_diff_time(philo->tvi, time), philo->nb, philo->nb - 1);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	return (1);
}

int	prendage_fork_two(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->fork[philo->nb % philo->nb_philo].mut);
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->fork[philo->nb % philo->nb_philo].mut);
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d has taken fork %d...\n",
		ft_diff_time(philo->tvi, time), philo->nb, philo->nb % philo->nb_philo);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	return (1);
}

int	mangeage(t_philo *philo, struct timeval *time)
{

	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(philo->tv, NULL);
	printf("%d ms : Philosopher %d is eating\n",
		ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	philo->eaten++;
	usleep(philo->time_to_eat * 1000);
	return (1);
}

int	lachage_fork_one(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d dropped fork %d...\n",
		ft_diff_time(philo->tvi, time), philo->nb, philo->nb - 1);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
	pthread_mutex_unlock(philo->fork[philo->nb - 1].mut);
	return (1);
}

int	lachage_fork_two(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->mut_end);
	if (philo->end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (0);
	}
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d dropped fork %d...\n",
		ft_diff_time(philo->tvi, time), philo->nb, philo->nb % philo->nb_philo);
	pthread_mutex_unlock(philo->mut_time);
	pthread_mutex_unlock(philo->print);
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
	pthread_mutex_unlock(philo->mut_end);
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->mut_time);
	gettimeofday(time, NULL);
	printf("%d ms : Philosopher %d is sleeping",
		ft_diff_time(philo->tvi, time), philo->nb);
	pthread_mutex_unlock(philo->mut_time);
	if (philo->eaten == philo->to_eat)
		printf(" forever.............\n");
	else
		printf("\n");
	pthread_mutex_unlock(philo->print);
	if (philo->eaten == philo->to_eat)
	{
		pthread_mutex_lock(philo->mut_end);
		philo->end = 1;
		pthread_mutex_unlock(philo->mut_end);
	}
	usleep(philo->time_to_sleep * 1000);
	return (1);
}

void	*ft_philo(void *param)
{
	struct timeval	*time;
	t_philo			*philo;

	philo = (t_philo *) param;
	time = malloc(sizeof(struct timeval));
	if (time == NULL)
		return (NULL);
	pthread_mutex_lock(philo->mut_end);
	while ((philo->to_eat == -1
			|| philo->eaten < philo->to_eat) && philo->end == 0)
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
		if (philo->nb % 2)
		{
			lachage_fork_one(philo, time);
			lachage_fork_two(philo, time);
		}
		else
		{
			lachage_fork_two(philo, time);
			lachage_fork_one(philo, time);
		}
		dormissage(philo, time);
		pthread_mutex_lock(philo->mut_end);
	}
	pthread_mutex_unlock(philo->mut_end);
	time = (free(time), NULL);
	pthread_mutex_lock(philo->mut_time);
	philo->tv = (free(philo->tv), NULL);
	pthread_mutex_unlock(philo->mut_time);
	return (NULL);
}

int	ft_atoi(char *s)
{
	int	ret;

	ret = 0;
	while (*s >= '0' && *s <= '9')
		ret = ret * 10 + (*s++ - 48);
	return (ret);
}

int	main(int ac, char **av)
{
	//Declarations
	int				number_of_philosophers;
	int				number_of_times_each_philosopher_must_eat;
	t_data			data;
	int				i;
	int				ret = 0;
	pthread_mutex_t	print;
	struct timeval	*time;

	//Initiation des mutex
	
	if (ac < 5 || ac > 6)
		return(printf("Mauvais nombre d'arguments\n"), 0);
	pthread_mutex_init(&data.print, NULL);
	pthread_mutex_init(&data.mut_nb_philo, NULL);
	pthread_mutex_init(&data.mut_end, NULL);
	pthread_mutex_init(&data.mut_time, NULL);

	//Lecture des arguments et initialisation de variables
	data.nb_philo = ft_atoi(av[1]);
	number_of_philosophers = data.nb_philo;
	data.end = 0;
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		number_of_times_each_philosopher_must_eat = atoi(av[5]);
	else
		number_of_times_each_philosopher_must_eat = -1;
	time = malloc(sizeof(struct timeval));
	if (!time)
		return (1);
	data.philo = malloc(sizeof(t_philo) * data.nb_philo);
	if (!data.philo)
		return (free(time), 1);
	if (!data.philo)
		return(EXIT_FAILURE);
	data.fork = malloc(sizeof(t_fork) * data.nb_philo);
	if (!data.fork)
		return(free(data.philo), free(time), EXIT_FAILURE);
	data.threads = calloc(sizeof(pthread_t), data.nb_philo);
	if (!data.threads)
		return(free(data.philo), free(data.fork), free(time), EXIT_FAILURE);
	i = -1;
	while (++i < data.nb_philo)
	{
		data.fork[i].nb = 0;
		data.fork[i].mut = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(data.fork[i].mut, NULL);
	}
	i = -1;
	gettimeofday(time, NULL);
	while (++i < data.nb_philo)
	{
		data.philo[i].fork = data.fork;
		data.philo[i].nb_philo = number_of_philosophers;
		data.philo[i].state = thinking;
		data.philo[i].end = 0;
		data.philo[i].time_to_die = data.time_to_die;
		data.philo[i].time_to_eat = data.time_to_eat;
		data.philo[i].time_to_sleep = data.time_to_sleep;
		data.philo[i].print = &data.print;
		data.philo[i].mut_end = &data.mut_end;
		data.philo[i].mut_time = &data.mut_time;
		data.philo[i].tv = malloc(sizeof(struct timeval));
		data.philo[i].nb = i + 1;
		data.philo[i].eaten = 0;
		data.philo[i].to_eat = number_of_times_each_philosopher_must_eat;
		data.philo[i].tvi = time;
	}
	i = -1;
	while (++i < data.nb_philo)
	{
		gettimeofday(data.philo[i].tv, NULL);
		//Peut etre pas NULL
		ret = pthread_create(&(data.threads[i]), NULL, ft_philo, &data.philo[i]);
		//ret = 0;
		if (ret != 0)
		{
			printf("Error %d\n", i);
			//	break;
			//Tout detruire et return.
			return (1);
		}
	}
	ret = pthread_create(&data.death_thread, NULL, death_checker, &data);
	i = -1;
	pthread_join(data.death_thread, NULL);
	pthread_mutex_destroy(&data.print);
	while (++i < data.nb_philo)
	{
		pthread_mutex_destroy(data.fork[i].mut);
		free(data.fork[i].mut);
		pthread_join(data.threads[i], NULL);
		//gerer erreurs
	}
	free(time);
	free(data.philo);
	free(data.fork);
	free(data.threads);
}
