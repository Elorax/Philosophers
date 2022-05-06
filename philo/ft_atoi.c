/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiersoh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:48:58 by abiersoh          #+#    #+#             */
/*   Updated: 2022/05/05 19:53:09 by abiersoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

int	ft_atoi(char *s)
{
	int	ret;

	ret = 0;
	while (*s >= '0' && *s <= '9')
		ret = ret * 10 + (*s++ - 48);
	return (ret);
}

int	is_arg_valid(char *s)
{
	long long int	res;

	while (*s && (*s == '\f' || *s == '\t' || *s == '\n'
			|| *s == '\r' || *s == '\v' || *s == ' '))
		s++;
	if (!(*s))
		return (0);
	res = 0;
	while ((*s && *s >= '0' && *s <= '9'))
	{
		res = (res * 10) + (*s - '0');
		if (res > 2147483647)
			return (0);
		s++;
	}
	return (*s == 0);
}
