/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:43 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:22:11 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Philosopher routine implementation for the dining philosophers simulation.
 *
 * This file contains the core logic executed by each philosopher thread,
 * including actions such as thinking, eating, sleeping, and handling forks.
 * It also manages synchronization mechanisms to avoid race conditions and deadlocks.
 */

#include "philo.h"

/**
 * @brief Arranges philosophers for initial thinking before starting their routine.
 *
 * This function staggers philosopher actions to reduce contention when
 * acquiring forks, improving simulation efficiency.
 *
 * - The first philosopher (ID 0) waits longer to start.
 * - Odd-indexed philosophers start thinking before picking up forks.
 * - Even-indexed philosophers also stagger their thinking phase.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	self_arrange(t_philo *p)
{
	bool	is_odd_philo;
	bool	is_even_philo;

	is_odd_philo = (p->num_philo & 1);
	is_even_philo = !(p->num_philo & 1);
	if (is_odd_philo && p->id == 0)
	{
		print_status(p, "is thinking");
		precise_sleep(p->eat_time << 1);
	}
	if (is_odd_philo && (p->id & 1))
	{
		print_status(p, "is thinking");
		precise_sleep(p->eat_time);
	}
	else if (is_even_philo && p->id & 1)
	{
		print_status(p, "is thinking");
		precise_sleep(p->eat_time);
	}
}

/**
 * @brief Executes the repeated routine of a philosopher.
 *
 * The routine consists of:
 * - Taking forks
 * - Eating (updates last meal time and meal count)
 * - Sleeping
 * - Thinking before repeating the process.
 *
 * Thread safety:
 * - Uses `meal_mutex` to safely update the last meal time and meal count.
 * - Uses mutex locks when taking and releasing forks to prevent race conditions.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	repeat_routine(t_philo *p)
{
	take_forks(p);
	pthread_mutex_lock(&p->env->meal_mutex);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->env->meal_mutex);
	print_status(p, "is eating");
	precise_sleep(p->eat_time);
	pthread_mutex_lock(&p->env->meal_mutex);
	p->meals++;
	pthread_mutex_unlock(&p->env->meal_mutex);
	put_forks(p);
	print_status(p, "is sleeping");
	precise_sleep(p->sleep_time);
	print_status(p, "is thinking");
}

/**
 * @brief Handles the special case where only one philosopher exists.
 *
 * A single philosopher picks up a fork and is unable to eat, leading to starvation.
 * This function ensures that the philosopher "dies" after the defined `die_time`.
 *
 * @param p Pointer to the philosopher structure.
 */

static void	process_single_philo(t_philo *p)
{
	print_status(p, "has taken a fork");
	usleep(p->die_time);
}

/**
 * @brief Ensures all threads start at the same simulation time.
 *
 * This function makes philosophers wait until the global `start_time`
 * is reached before beginning execution.
 *
 * Thread safety:
 * - Uses `start_mutex` to synchronize all philosophers' starting point.
 *
 * @param p Pointer to the philosopher structure.
 */

static void	wait_all_threads(t_philo *p)
{
	pthread_mutex_lock(&p->env->start_mutex);
	while (get_time() < p->env->start_time)
		usleep(50);
	pthread_mutex_unlock(&p->env->start_mutex);
}

/**
 * @brief The main routine executed by each philosopher thread.
 *
 * This function manages a philosopher’s lifecycle, ensuring synchronization
 * and termination conditions are met. It follows these steps:
 *
 * - Waits for all threads to start simultaneously.
 * - Handles the special case for a single philosopher.
 * - Arranges initial thinking order to reduce contention.
 * - Enters a loop where the philosopher eats, sleeps, and thinks.
 * - Checks termination conditions (death or meal limit).
 *
 * Thread safety:
 * - Uses `end_mutex` to check and update the simulation termination state.
 * - Uses `meal_mutex` to track meals and last meal timestamps.
 * - Uses `start_mutex` to synchronize all threads' starting time.
 *
 * @param arg Pointer to the philosopher structure (`t_philo`).
 * @return NULL when the philosopher exits.
 */
void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	wait_all_threads(p);
	if (p->num_philo == 1)
	{
		process_single_philo(p);
		return (NULL);
	}
	self_arrange(p);
	while (1)
	{
		pthread_mutex_lock(&p->env->end_mutex);
		if (p->env->ended
			|| (p->meals_limit != -1 && p->meals >= p->meals_limit))
		{
			pthread_mutex_unlock(&p->env->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&p->env->end_mutex);
		repeat_routine(p);
		usleep(500);
	}
	return (NULL);
}
