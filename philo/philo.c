/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 21:15:28 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	main(int ac, char **av)
{
	t_data			data;
	struct timeval	*time;

	read_args(ac);
	set_data(ac, av, &data);
	verif_input(ac, av, &data);
	time = malloc(sizeof(struct timeval));
	if (!time)
		return (1);
	allocate(&data, time);
	init_mutex(&data, time);
	init_philo(&data, time);
	quit_crash(&data, time);
	create_threads(&data);
	join_threads(&data);
	destroy_data_mutex(&data, time);
	return (0);
}
