/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/11 19:44:10 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	main(int ac, char **av)
{
	t_data			data;
	struct timeval	time;

	read_args(ac);
	set_data(ac, av, &data);
	verif_input(ac, av, &data);
	allocate(&data, &time);
	init_mutex(&data, &time);
	init_philo(&data, &time);
	quit_crash(&data);
	create_threads(&data);
	if (data.crash)
		quit_crash(&data);
	join_threads(&data);
	destroy_data_mutex(&data);
	return (0);
}
