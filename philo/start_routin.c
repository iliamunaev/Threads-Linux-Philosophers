/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_routin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:44:14 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:18:20 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file start_routin.c
 * @brief Starts philosopher and monitor threads.
 */
#include "philo.h"

/**
 * @brief Creates philosopher and monitor threads.
 *
 * This function initializes philosopher threads and a monitor thread.
 * It ensures that all philosopher threads start execution before unlocking
 * the simulation start mutex.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the monitor thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int	start_routin(t_env *env, pthread_t *mon)
{
	int	i;

	pthread_mutex_lock(&env->start_mutex);
	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: start_routin: failed to create philo thread\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}
