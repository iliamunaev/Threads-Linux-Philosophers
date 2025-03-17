/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:43 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:09:02 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* @file philo.c
* @brief Philosopher routine implementation.
*/
#include "philo.h"

/**
* @brief Arranges philosophers for thinking before starting routine.
*
* Handles initial thinking time for certain philosophers to avoid contention.
*
* @param p Pointer to the philosopher structure.
*/
static void	self_arrange(t_philo *p)
{
	if (p->env->num_philo % 2 && p->id == 0)
	{
		print_status(p, "is thinking");
		precise_sleep(2 * p->env->eat_time);
	}
	if (p->env->num_philo % 2 && p->id % 2)
	{
		print_status(p, "is thinking");
		precise_sleep(p->env->eat_time);
	}
	else if (((p->env->num_philo % 2) == 0) && p->id % 2)
	{
		print_status(p, "is thinking");
		precise_sleep(p->env->eat_time);
	}
}

/**
 * @brief Executes the repeated routine of a philosopher.
 *
 * This includes taking forks, eating, sleeping, and thinking.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	repeat_routine(t_philo *p)
{
	take_forks(p);
	pthread_mutex_lock(&p->env->meal_mutex);
	p->last_meal = get_time();
	p->meals++;
	pthread_mutex_unlock(&p->env->meal_mutex);
	print_status(p, "is eating");
	precise_sleep(p->env->eat_time);
	put_forks(p);
	print_status(p, "is sleeping");
	precise_sleep(p->env->sleep_time);
	print_status(p, "is thinking");
}

/**
 * @brief Handles the case where there is only one philosopher.
 *
 * Since a single philosopher cannot acquire two forks, they just
 * think until they die.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	process_single_philo(t_philo *p)
{
	print_status(p, "is thinking");
	usleep(p->env->die_time + 5);
}

/**
 * @brief Philosopher thread routine.
 *
 * This function defines the philosopher's behavior in a separate thread.
 * If there is only one philosopher, a special handling function is called.
 * Otherwise, they follow a normal routine of eating, sleeping, and thinking.
 *
 * @param arg Pointer to the philosopher structure.
 * @return NULL when the philosopher stops execution.
 */
void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->env->num_philo == 1)
	{
		process_single_philo(p);
		return (NULL);
	}
	self_arrange(p);
	while (1)
	{
		pthread_mutex_lock(&p->env->end_mutex);
		if (p->env->ended || (p->env->meals_limit != -1 && p->meals >= p->env->meals_limit))
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
