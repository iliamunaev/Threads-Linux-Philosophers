/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:43 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 13:55:33 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* @file philo.c
* @brief Philosopher routine implementation.
*/
#include "philo.h"

static void	self_arrange(t_philo *p)
{
	bool is_odd_philo;
	bool is_even_philo;

	is_odd_philo = (p->num_philo & 1);
	is_even_philo = !(p->num_philo & 1);
	if (is_odd_philo && p->id == 0)
	{
		print_status(p, "is thinking");
		precise_sleep(p->eat_time << 1);
	}
	if (is_odd_philo&& (p->id & 1))
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

static void	process_single_philo(t_philo *p)
{
	print_status(p, "has taken a fork");
	usleep(p->die_time);
}

static void wait_all_threads(t_philo *p)
{
	pthread_mutex_lock(&p->env->start_mutex);
	while (get_time() < p->env->start_time)
		usleep(50);
	pthread_mutex_unlock(&p->env->start_mutex);
}

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
