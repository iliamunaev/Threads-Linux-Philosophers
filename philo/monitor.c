/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:56:20 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:07:54 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file monitor.c
 * @brief Monitoring functions for philosopher simulation.
 */
#include "philo.h"

/**
 * @brief Checks if a philosopher has died.
 *
 * This function calculates the time since a philosopher's last meal
 * and compares it with the allowed die time. If the philosopher has
 * exceeded the time limit, it marks the simulation as ended and prints
 * a death message.
 *
 * @param env Pointer to the environment structure.
 * @param i Index of the philosopher to check.
 * @return 1 if a philosopher has died, otherwise 0.
 */

static int	check_death(t_env *env, int i)
{
	long	time_since_meal;

	pthread_mutex_lock(&env->meal_mutex);
	time_since_meal = get_time() - env->philos[i].last_meal;
	pthread_mutex_unlock(&env->meal_mutex);
	if (time_since_meal > env->die_time)
	{
		pthread_mutex_lock(&env->end_mutex);
		if (!env->ended)
			env->ended = 1;
		pthread_mutex_unlock(&env->end_mutex);
		pthread_mutex_lock(&env->print_mutex);
		printf("%ld %d died\n", get_time() - env->start_time, i + 1);
		pthread_mutex_unlock(&env->print_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if all philosophers have eaten enough meals.
 *
 * This function verifies if every philosopher has reached the required
 * meal limit. If all have, it returns 1, indicating the simulation should end.
 *
 * @param env Pointer to the environment structure.
 * @return 1 if all philosophers have eaten enough, otherwise 0.
 */
static int	check_full(t_env *env)
{
	int	i;
	int	full_count;

	if (env->meals_limit == -1)
		return (0);
	full_count = 0;
	i = 0;
	while (i < env->num_philo)
	{
		pthread_mutex_lock(&env->meal_mutex);
		if (env->philos[i].meals >= env->meals_limit)
			full_count++;
		pthread_mutex_unlock(&env->meal_mutex);
		i++;
	}
	return (full_count == env->num_philo);
}

/**
 * @brief Monitor thread function.
 *
 * Continuously checks for philosopher deaths or if all philosophers
 * have finished eating. Ends the simulation when necessary.
 *
 * @param arg Pointer to the environment structure.
 * @return NULL when the monitoring thread exits.
 */
void	*monitor(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	while (1)
	{
		pthread_mutex_lock(&env->end_mutex);
		if (env->ended)
		{
			pthread_mutex_unlock(&env->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&env->end_mutex);
		i = 0;
		while (i < env->num_philo)
		{
			if (check_death(env, i))
			{
				return (NULL);
			}
			i++;
		}
		if (check_full(env))
		{
			pthread_mutex_lock(&env->end_mutex);
			env->ended = 1;
			pthread_mutex_unlock(&env->end_mutex);
		}
		usleep(5000);
	}
	return (NULL);
}
