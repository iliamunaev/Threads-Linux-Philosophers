/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:08:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:10:03 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_print_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->print_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_meal_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->meal_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
