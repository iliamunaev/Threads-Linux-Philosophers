/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:41 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:05:44 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo_routine.c
 * @brief Implements philosopher actions and timing functions.
 */
#include "philo.h"

/**
 * @brief Gets the current timestamp in milliseconds.
 *
 * Uses gettimeofday() to retrieve the current time and convert it into
 * milliseconds.
 *
 * @return Current time in milliseconds.
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * @brief Sleeps for a precise amount of milliseconds.
 *
 * Uses a loop to continuously check elapsed time and sleep in short
 * intervals to maintain precision.
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
 * @brief Philosopher takes both forks before eating.
 *
 * Implements a strategy where even-indexed philosophers pick up
 * the left fork first,
 * while odd-indexed philosophers pick up the right fork first to
 * reduce deadlock risk.
 *
 * @param p Pointer to the philosopher structure.
 */
void	take_forks(t_philo *p)
{
	int	left;
	int	right;

	left = p->id;
	right = (p->id + 1) % p->env->num_philo;
	if (p->id % 2 == 0)
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
 * @brief Philosopher releases both forks after eating.
 *
 * Unlocks the mutexes associated with the philosopher's left and right forks.
 *
 * @param p Pointer to the philosopher structure.
 */
void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->env->forks[p->id]);
	pthread_mutex_unlock(&p->env->forks[(p->id + 1) % p->env->num_philo]);
}
