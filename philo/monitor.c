/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:31:00 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:15:39 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file monitor.c
 * @brief Monitoring functions for the philosopher simulation.
 *
 * This file contains functions responsible for monitoring philosopher status,
 * including detecting deaths and checking if all philosophers have finished
 * eating.
 * The monitor thread ensures that the simulation ends when necessary.
 */

#include "philo.h"

/**
 * @brief Checks if a philosopher has died.
 *
 * This function calculates the time since a philosopher's last meal and
 * compares
 * it with the allowed die time. If the philosopher has exceeded the time limit,
 * the function marks the simulation as ended and prints a death message.
 *
 * Thread safety:
 * - Uses `meal_mutex` to safely access each philosopher's last meal time.
 * - Uses `end_mutex` to update the simulation termination flag.
 * - Uses `print_mutex` to print the death message without race conditions.
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
 * This function verifies if every philosopher has reached the required meal
 * limit.
 * If all philosophers have consumed the required number of meals, it marks the
 * simulation as ended.
 *
 * Thread safety:
 * - Uses `meal_mutex` to safely access meal counts.
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
 * @brief Checks if the simulation should terminate.
 *
 * This function checks the `ended` flag to determine if the simulation has been
 * marked for termination. It ensures thread-safe access to this shared flag.
 *
 * Thread safety:
 * - Uses `end_mutex` to safely read the `ended` flag.
 *
 * @param env Pointer to the environment structure.
 * @return 1 if the simulation should end, otherwise 0.
 */
static int	should_terminate(t_env *env)
{
	pthread_mutex_lock(&env->end_mutex);
	if (env->ended)
	{
		pthread_mutex_unlock(&env->end_mutex);
		return (1);
	}
	pthread_mutex_unlock(&env->end_mutex);
	return (0);
}

/**
 * @brief Monitor thread function.
 *
 * This function continuously checks if a philosopher has died or if all
 * philosophers have eaten enough meals. If either condition is met, it
 * marks the simulation as ended and exits the thread.
 *
 * Thread safety:
 * - Uses `end_mutex`, `meal_mutex`, and `print_mutex` to ensure safe access
 *   to shared data.
 *
 * @param arg Pointer to the environment structure (`t_env`).
 * @return NULL when the monitoring thread exits.
 */
void	*monitor(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	while (1)
	{
		usleep(5000);
		if (should_terminate(env))
			break ;
		i = 0;
		while (i < env->num_philo)
		{
			if (check_death(env, i))
				return (NULL);
			i++;
		}
		if (check_full(env))
		{
			pthread_mutex_lock(&env->end_mutex);
			env->ended = 1;
			pthread_mutex_unlock(&env->end_mutex);
		}
	}
	return (NULL);
}
