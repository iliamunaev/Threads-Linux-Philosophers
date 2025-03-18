/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:50:05 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:11:29 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file join_threads.c
 * @brief Thread management for the philosopher simulation.
 *
 * This file contains the function responsible for joining all created
 * threads, ensuring proper synchronization and cleanup before exiting
 * the program.
 */

#include "philo.h"

/**
 * @brief Waits for all created threads to finish execution.
 *
 * This function ensures that all threads (philosophers, monitor, and logger)
 * are properly joined before the program exits, preventing orphaned threads
 * and ensuring safe resource cleanup.
 *
 * - If philosopher threads were created, it joins each philosopher thread.
 * - If the monitor thread was created, it joins the monitor thread.
 * - If the logger thread was created, it joins the logger thread.
 *
 * @param env Pointer to the environment structure (`t_env`).
 * @param mon Monitor thread identifier.
 * @param logger_thread Logger thread identifier.
 */
void	join_threads(t_env *env, pthread_t mon, pthread_t logger_thread)
{
	int	i;

	if (!env)
		return ;
	if (env->t_philos_created)
	{
		i = 0;
		while (i < env->num_philo)
		{
			pthread_join(env->philos[i].thread, NULL);
			i++;
		}
	}
	if (env->t_mon_created)
		pthread_join(mon, NULL);
	if (env->t_logger_created)
		pthread_join(logger_thread, NULL);
}
