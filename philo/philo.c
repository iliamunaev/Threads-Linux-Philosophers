/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:43 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 15:42:00 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	process_single_philo(t_philo *p)
{
		print_status(p, "is thinking");
		usleep(p->env->die_time + 5);
}
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
