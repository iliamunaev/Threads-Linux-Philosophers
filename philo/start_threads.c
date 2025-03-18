/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:46:19 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:13:34 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	env->start_time = get_time();
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}
