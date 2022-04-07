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

typedef struct	s_data
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



int	ft_diff_time(struct timeval *t1, struct timeval *t2);

void	check_finish(t_data *data);

void	*death_checker(void *param);//A normer

int	pensage(t_philo *philo, struct timeval *time);
int	attendage(t_philo *philo);
int	prendage_fork_one(t_philo *philo, struct timeval *time);
int	prendage_fork_two(t_philo *philo, struct timeval *time);
int	lachage_fork_one(t_philo *philo, struct timeval *time);
int	lachage_fork_two(t_philo *philo, struct timeval *time);
int	mangeage(t_philo *philo, struct timeval *time);
int	dormissage(t_philo *philo, struct timeval *time);

void	boucle_philo(t_philo *philo, struct timeval *time);
void	*ft_philo(void *param);



#endif
