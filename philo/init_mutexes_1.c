/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:08:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:16:01 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_mutexes_1.c
 * @brief Mutex initialization functions for the philosopher simulation.
 *
 * This file contains functions for initializing mutexes used for
 * synchronization in the simulation, including control over start,
 * end conditions, logging, and philosopher fork access.
 */

#include "philo.h"

/**
 * @brief Initializes the start synchronization mutex.
 *
 * This mutex ensures that all philosophers start at the same time.
 * If initialization fails, it destroys previously initialized mutexes
 * (`print_mutex` and `meal_mutex`) to prevent resource leaks.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS on success, or EXIT_FAILURE if
 * initialization fails.
 */
int	init_start_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->start_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the end synchronization mutex.
 *
 * This mutex ensures safe access to the simulation's termination flag.
 * If initialization fails, it destroys previously initialized mutexes
 * to clean up allocated resources.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS on success, or EXIT_FAILURE if
 * initialization fails.
 */
int	init_end_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->end_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		(void)pthread_mutex_destroy(&env->start_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the log buffer mutex.
 *
 * This mutex protects access to the log buffer to prevent race conditions
 * when multiple threads attempt to write logs simultaneously. If initialization
 * fails, it destroys previously initialized mutexes to clean up.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS on success, or EXIT_FAILURE if
 * initialization fails.
 */

int	init_log_buffer_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->log_buffer.mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		(void)pthread_mutex_destroy(&env->start_mutex);
		(void)pthread_mutex_destroy(&env->end_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes mutexes for philosopher forks.
 *
 * Each philosopher has a corresponding fork mutex to control access
 * to shared resources. If initialization fails, previously initialized
 * fork mutexes are destroyed to prevent resource leaks. Additionally,
 * all other environment-related mutexes are also destroyed to ensure
 * a complete cleanup.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS if all fork mutexes are initialized,
 * otherwise returns EXIT_FAILURE.
 */
int	init_forks_mutex(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_mutex_init(&env->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				(void)pthread_mutex_destroy(&env->forks[i]);
			}
			(void)pthread_mutex_destroy(&env->print_mutex);
			(void)pthread_mutex_destroy(&env->meal_mutex);
			(void)pthread_mutex_destroy(&env->start_mutex);
			(void)pthread_mutex_destroy(&env->end_mutex);
			(void)pthread_mutex_destroy(&env->log_buffer.mutex);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
