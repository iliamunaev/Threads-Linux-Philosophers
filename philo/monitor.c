/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:56:20 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 13:33:50 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_env *env, int i)
{
	long	time_since_meal;

	pthread_mutex_lock(&env->meal_mutex);
	time_since_meal = get_time() - env->philos[i].last_meal;
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
		return (1);
	}
	return (0);
}

static int	check_full(t_env *env)
{
	int	i;
	int	full_count;

	if (env->meals_limit == -1)
		return (0);	
	full_count = 0;
	i = 0;
	while(i < env->num_philo)
	{
		pthread_mutex_lock(&env->meal_mutex);
		if (env->philos[i].meals >= env->meals_limit)
			full_count++;
		pthread_mutex_unlock(&env->meal_mutex);
		i++;
	}
	return (full_count == env->num_philo);
}

void	*monitor(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	while (!env->ended)
	{
		i = 0;
		while (i < env->num_philo)
		{
			if (check_death(env, i))
				return (NULL);
			i++;
		}
		if (check_full(env))
			env->ended = 1;
		usleep(5000);
	}
	return (NULL);
}
