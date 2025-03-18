/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_managment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:05:25 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:28:57 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file memory_managment.c
 * @brief Memory and resource cleanup utilities for the philosopher simulation.
 *
 * This file provides functions to release allocated memory and
 * destroy synchronization primitives (mutexes) to ensure proper cleanup.
 */

#include "philo.h"

/**
 * @brief Destroys all mutexes used in the simulation.
 *
 * This function safely destroys all mutexes associated with the environment,
 * including those used for printing, meal tracking, synchronization,
 * and philosopher forks.
 *
 * @param env Pointer to the environment structure containing mutexes.
 */
void	destroy_mutexes(t_env *env)
{
	int	i;

	if (!env)
		return ;
	(void)pthread_mutex_destroy(&env->print_mutex);
	(void)pthread_mutex_destroy(&env->meal_mutex);
	(void)pthread_mutex_destroy(&env->start_mutex);
	(void)pthread_mutex_destroy(&env->end_mutex);
	(void)pthread_mutex_destroy(&env->log_buffer.mutex);
	i = 0;
	while (i < env->num_philo)
	{
		(void)pthread_mutex_destroy(&env->forks[i]);
		i++;
	}
}

/**
 * @brief Frees allocated memory for environment structures.
 *
 * This function releases memory allocated for forks and philosopher structures,
 * ensuring that all dynamically allocated resources are properly freed.
 *
 * @param env Pointer to the environment structure.
 */
void	free_env(t_env *env)
{
	if (!env)
		return ;
	if (env->forks)
	{
		free(env->forks);
		env->forks = NULL;
	}
	if (env->philos)
	{
		free(env->philos);
		env->philos = NULL;
	}
	free(env);
	env = NULL;
}

/**
 * @brief Cleans up all resources used in the simulation.
 *
 * This function calls `destroy_mutexes()` to release all mutexes
 * and `free_env()` to free allocated memory, ensuring a complete cleanup.
 *
 * @param env Pointer to the environment structure.
 */
void	free_all(t_env *env)
{
	if (!env)
		return ;
	destroy_mutexes(env);
	free_env(env);
}
