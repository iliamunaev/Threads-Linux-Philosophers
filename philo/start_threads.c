/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:46:19 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:16:41 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file start_threads.c
 * @brief Thread creation and management for the philosopher simulation.
 *
 * This file contains functions responsible for creating threads for
 * philosophers, the monitor, and the logger. It ensures proper thread
 * synchronization and handles failures gracefully.
 */

#include "philo.h"

/**
 * @brief Creates the logger thread.
 *
 * The logger thread (`log_flusher`) is responsible for periodically
 * flushing buffered log entries to standard output.
 *
 * If thread creation fails, the function prints an error message.
 *
 * Thread safety:
 * - No shared data is modified, so no mutex is required.
 *
 * @param env Pointer to the environment structure.
 * @param logger_thread Pointer to the `pthread_t` variable for the logger
 * thread.
 * @return int Returns EXIT_SUCCESS if the thread is created successfully,
 * otherwise EXIT_FAILURE.
 */
static int	create_logger_thread(t_env *env, pthread_t *logger_thread)
{
	if (pthread_create(logger_thread, NULL, &log_flusher, env) != 0)
	{
		print_error("Error: Failed to create logger thread\n");
		return (EXIT_FAILURE);
	}
	env->t_logger_created = true;
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates the monitor thread.
 *
 * The monitor thread continuously checks for philosopher deaths and
 * whether all philosophers have eaten enough meals. If thread creation
 * fails, it sets `env->ended` to true to signal termination.
 *
 * Thread safety:
 * - Uses `end_mutex` to safely update the `ended` flag.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the `pthread_t` variable for the monitor thread.
 * @return int Returns EXIT_SUCCESS if the thread is created successfully,
 * otherwise EXIT_FAILURE.
 */
static int	create_monitor_thread(t_env *env, pthread_t *mon)
{
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		pthread_mutex_lock(&env->end_mutex);
		env->ended = true;
		pthread_mutex_unlock(&env->end_mutex);
		return (EXIT_FAILURE);
	}
	env->t_mon_created = true;
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates philosopher threads.
 *
 * This function initializes a thread for each philosopher to run their
 * routine concurrently. If a thread fails to create, it sets `env->ended`
 * to true and joins all previously created philosopher threads to ensure
 * proper cleanup.
 *
 * Thread safety:
 * - Uses `end_mutex` to safely update the `ended` flag upon failure.
 *
 * @param env Pointer to the environment structure.
 * @return int Returns EXIT_SUCCESS if all threads are created successfully,
 * otherwise EXIT_FAILURE.
 */
static int	create_philosopher_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL,
				routine, &env->philos[i]) != 0)
		{
			print_error("Error: Failed to create philosopher thread\n");
			pthread_mutex_lock(&env->end_mutex);
			env->ended = true;
			pthread_mutex_unlock(&env->end_mutex);
			while (--i >= 0)
				pthread_join(env->philos[i].thread, NULL);
			return (EXIT_FAILURE);
		}
		i++;
	}
	env->t_philos_created = true;
	return (EXIT_SUCCESS);
}

/**
 * @brief Starts all necessary threads for the simulation.
 *
 * This function creates the logger, monitor, and philosopher threads
 * while ensuring proper synchronization. If any thread creation fails,
 * it releases the `start_mutex` and returns an error.
 *
 * Steps:
 * - Creates the logger thread.
 * - Creates the monitor thread.
 * - Creates philosopher threads.
 * - Unlocks `start_mutex` and sets `start_time` when all threads are ready.
 *
 * Thread safety:
 * - Uses `start_mutex` to synchronize thread start timing.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the `pthread_t` variable for the monitor thread.
 * @param logger_thread Pointer to the `pthread_t` variable for the logger
 * thread.
 * @return int Returns EXIT_SUCCESS if all threads start successfully,
 * otherwise EXIT_FAILURE.
 */
int	start_threads(t_env *env, pthread_t *mon, pthread_t *logger_thread)
{
	pthread_mutex_lock(&env->start_mutex);
	if (create_logger_thread(env, logger_thread) == EXIT_FAILURE)
	{
		pthread_mutex_unlock(&env->start_mutex);
		return (EXIT_FAILURE);
	}
	if (create_monitor_thread(env, mon) == EXIT_FAILURE)
	{
		pthread_mutex_unlock(&env->start_mutex);
		return (EXIT_FAILURE);
	}
	if (create_philosopher_threads(env) == EXIT_FAILURE)
	{
		pthread_mutex_unlock(&env->start_mutex);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}
