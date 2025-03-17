/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:45:11 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:04:54 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file clean_up.c
 * @brief Cleanup functions for philosopher simulation.
 */
#include "philo.h"

/**
 * @brief Frees allocated memory and destroys mutexes.
 *
 * This function ensures that all dynamically allocated resources
 * and mutexes in the environment structure are properly freed and destroyed.
 *
 * @param env Pointer to the environment structure.
 */
void	clean_up(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	if (env->forks)
	{
		while (i < env->num_philo)
		{
			pthread_mutex_destroy(&env->forks[i]);
			i++;
		}
		free(env->forks);
	}
	if (env->philos)
		free(env->philos);
	if (env->ticket_nums)
		free(env->ticket_nums);
	pthread_mutex_destroy(&env->end_mutex);
	pthread_mutex_destroy(&env->print_mutex);
	pthread_mutex_destroy(&env->meal_mutex);
	pthread_mutex_destroy(&env->start_mutex);
	pthread_mutex_destroy(&env->log_buffer.mutex);
	free(env);
}
