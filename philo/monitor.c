/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:56:20 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 13:11:50 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *arg)
{
	t_env	*env;
	int		full;
	int		i;
	long	time_since_meal;
	int		mcount;

	env = (t_env *)arg;
	while (!env->ended)
	{
		full = 0;
		i = 0;
		while (i < env->num_philo)
		{
			pthread_mutex_lock(&env->meal_mutex);
			time_since_meal = get_time() - env->philos[i].last_meal;
			mcount = env->philos[i].meals;
			pthread_mutex_unlock(&env->meal_mutex);
			if (time_since_meal > env->die_time)
			{
				pthread_mutex_lock(&env->print_mutex);
				if (!env->ended)
				{
					env->ended = 1;
					printf("%ld %d died\n", get_time() - env->start_time, i + 1);
				}
				pthread_mutex_unlock(&env->print_mutex);
				return (NULL);
			}
			if (env->meals_limit != -1 && mcount >= env->meals_limit)
				full++;
			i++;
		}
		if (env->meals_limit != -1 && full == env->num_philo)
			env->ended = 1;
		usleep(5000);
	}
	return (NULL);
}