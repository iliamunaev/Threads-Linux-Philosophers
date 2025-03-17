/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:46:19 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:48:33 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Creates a logger thread.
 *
 * @param env Pointer to the environment structure.
 * @param logger_thread Pointer to the logger thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_logger_thread(t_env *env, pthread_t *logger_thread)
{
	if (pthread_create(logger_thread, NULL, &log_flusher, env) != 0)
	{
		print_error("Error: Failed to create logger thread\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates philosopher threads.
 *
 * @param env Pointer to the environment structure.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_philosopher_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread,
				NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: Failed to create philosopher thread\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates the monitor thread.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the monitor thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_monitor_thread(t_env *env, pthread_t *mon)
{
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Starts all required threads.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the monitor thread.
 * @param logger_thread Pointer to the logger thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int	start_threads(t_env *env, pthread_t *mon, pthread_t *logger_thread)
{
	pthread_mutex_lock(&env->start_mutex);
	if (create_logger_thread(env, logger_thread) == EXIT_FAILURE)
	{
		clean_up(env);
		return (EXIT_FAILURE);
	}
	if (create_philosopher_threads(env) == EXIT_FAILURE)
	{
		pthread_mutex_unlock(&env->start_mutex);
		clean_up(env);
		return (EXIT_FAILURE);
	}
	if (create_monitor_thread(env, mon) == EXIT_FAILURE)
	{
		clean_up(env);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}
