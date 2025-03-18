/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:08:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:16:29 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_mutexes_2.c
 * @brief Mutex initialization functions for shared resources in the
 * philosopher simulation.
 *
 * This file contains functions for initializing mutexes used for protecting
 * shared output (printing) and meal tracking.
 */

#include "philo.h"

/**
 * @brief Initializes the print mutex.
 *
 * This mutex ensures that only one thread at a time can write to standard
 * output, preventing interleaved log messages from different philosophers.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS on successful initialization, or
 * EXIT_FAILURE if mutex initialization fails.
 */
int	init_print_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->print_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the meal tracking mutex.
 *
 * This mutex protects access to philosopher meal tracking variables to prevent
 * race conditions when checking or updating the number of meals eaten.
 * If initialization fails, it cleans up by destroying the previously
 * initialized `print_mutex` to avoid resource leaks.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS on successful initialization, or
 * EXIT_FAILURE if mutex initialization fails.
 */
int	init_meal_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->meal_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
