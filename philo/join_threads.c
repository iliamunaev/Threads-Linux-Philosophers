/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:50:05 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:57:55 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file join_threads.c
 * @brief Handles buffered logging for philosopher simulation.
 */
#include "philo.h"

/**
 * @brief Joins all created threads.
 *
 * @param env Pointer to the environment structure.
 * @param mon Monitor thread.
 * @param logger_thread Logger thread.
 */
void	join_threads(t_env *env, pthread_t mon, pthread_t logger_thread)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		pthread_join(env->philos[i].thread, NULL);
		i++;
	}
	pthread_join(mon, NULL);
	pthread_join(logger_thread, NULL);
}
