/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 21:29:28 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 21:33:37 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

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
	int				end;
	pthread_mutex_t	*print;
	pthread_mutex_t	*mut_end;
	pthread_mutex_t	*mut_time;
	t_fork			*fork;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				end;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				crash;

	int				nb_to_eat;

	t_philo			*philo;
	t_fork			*fork;
	pthread_t		*threads;
	pthread_t		death_thread;
	pthread_mutex_t	print;
	pthread_mutex_t	mut_end;
	pthread_mutex_t	mut_time;
}	t_data;

void	ft_msleep(int msec, t_philo *philo);
int		is_arg_valid(char *s);

int		ft_diff_time(struct timeval *t1, struct timeval *t2);

void	check_finish(t_data *data);
void	print_death(t_data *data, struct timeval *time, int i);
void	check_deaths(t_data *data, struct timeval *time);

void	*death_checker(void *param);//A normer
void	free_tv(t_philo *philo, int n);

int		pensage(t_philo *philo, struct timeval *time);
int		attendage(t_philo *philo);
int		prendage_fork_one(t_philo *philo, struct timeval *time);
int		prendage_fork_two(t_philo *philo, struct timeval *time);
int		lachage_fork_one(t_philo *philo, struct timeval *time);
int		lachage_fork_two(t_philo *philo, struct timeval *time);
int		mangeage(t_philo *philo, struct timeval *time);
int		dormissage(t_philo *philo, struct timeval *time);

void	boucle_philo(t_philo *philo, struct timeval *time);
void	handle_solo_philo(t_philo *philo, struct timeval *time);
void	*ft_philo(void *param);

int		ft_atoi(char *s);

int		read_args(int ac);
int		set_data(int ac, char **av, t_data *data);
int		verif_input(int ac, char **av, t_data *data);
int		allocate(t_data *data, struct timeval *time);
int		init_mutex(t_data *data, struct timeval *time);
void	init_philo(t_data *data, struct timeval *time);
int		quit_crash(t_data *data, struct timeval *time);
int		create_threads(t_data *data);
int		join_threads(t_data *data);
int		destroy_data_mutex(t_data *data, struct timeval *time);

#endif
