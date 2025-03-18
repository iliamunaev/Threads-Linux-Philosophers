/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:12:22 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:19:09 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo_routin.c
 * @brief Philosopher routine functions for the dining philosophers simulation.
 *
 * This file contains utility functions that handle time management,
 * precise sleeping, and philosopher actions such as taking and releasing forks.
 */

#include "philo.h"

/**
 * @brief Retrieves the current timestamp in milliseconds.
 *
 * Uses `gettimeofday()` to fetch the current system time and converts
 * it into milliseconds for accurate time tracking in the simulation.
 *
 * Thread safety:
 * - This function does not use shared data, so no mutex is required.
 *
 * @return The current time in milliseconds, or -1 if `gettimeofday()` fails.
 */
long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		print_error("Error: get_time: gettimeofday failed.\n");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * @brief Sleeps for a precise duration in milliseconds.
 *
 * Uses a loop to repeatedly check the elapsed time while calling `usleep(500)`.
 * This ensures better precision compared to `usleep(ms * 1000)`, which may be
 * affected by system scheduling delays.
 *
 * Thread safety:
 * - This function does not modify shared data and does not require a mutex.
 *
 * @param ms The duration to sleep in milliseconds.
 */
void	precise_sleep(long ms)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(500);
}

/**
 * @brief Handles the action of a philosopher taking both forks before eating.
 *
 * Implements a strategy to reduce deadlocks:
 * - Even-indexed philosophers pick up their left fork first.
 * - Odd-indexed philosophers pick up their right fork first.
 *
 * Thread safety:
 * - Uses `pthread_mutex_lock()` to prevent race conditions when accessing forks.
 *
 * @param p Pointer to the philosopher structure.
 */
void	take_forks(t_philo *p)
{
	int	left;
	int	right;

	left = p->id;
	right = (p->id + 1) % p->num_philo;
	if (!(p->id & 1))
	{
		pthread_mutex_lock(&p->env->forks[left]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->env->forks[right]);
		print_status(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->env->forks[right]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->env->forks[left]);
		print_status(p, "has taken a fork");
	}
}

/**
 * @brief Handles the action of a philosopher releasing both forks after eating.
 *
 * Unlocks the mutexes associated with the philosopher's left and right forks.
 *
 * Thread safety:
 * - Uses `pthread_mutex_unlock()` to safely release fork resources.
 *
 * @param p Pointer to the philosopher structure.
 */
void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->env->forks[p->id]);
	pthread_mutex_unlock(&p->env->forks[(p->id + 1) % p->num_philo]);
}
