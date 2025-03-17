/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:45:11 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 15:38:30 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_up(t_env *env)
{
	int i;

	if (!env)
		return;
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
