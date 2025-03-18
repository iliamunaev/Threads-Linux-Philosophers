/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:05:25 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:05:44 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	free_all(t_env *env)
{
	if (!env)
		return ;
	destroy_mutexes(env);
	free_env(env);
}
