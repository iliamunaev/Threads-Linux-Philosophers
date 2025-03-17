/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:45:11 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 11:58:35 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*

typedef struct s_philo
{
	int		 id;
	int		 meals;
	long		last_meal;
	pthread_t   thread;
	t_env	   *env;
}   t_philo;


typedef struct s_env
{
	int			 num_philo;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	int			 meals_limit;
	int			 ended;
	long			start_time;
	pthread_mutex_t *forks;
	t_philo		 *philos;
	pthread_cond_t  *cond_vars;
	int			 *ticket_nums;
	int			 ticket_counter;
	pthread_mutex_t hunger_lock;
	pthread_mutex_t print_mutex;
	pthread_mutex_t meal_mutex;
	pthread_mutex_t start_mutex;
}   t_env;

*/
void clean_up(t_env *env)
{
	int i;

	if (!env)
		return;

	// Destroy mutexes for forks
	i = 0;
	if (env->forks)
	{
		while (i < env->num_philo)
		{
			pthread_mutex_destroy(&env->forks[i]);
			i++;
		}
		free(env->forks);
	}

	// Destroy condition variables
	i = 0;
	if (env->cond_vars)
	{
		while (i < env->num_philo)
		{
			pthread_cond_destroy(&env->cond_vars[i]);
			i++;
		}
		free(env->cond_vars);
	}

	// Free philosophers array
	if (env->philos)
		free(env->philos);

	// Free ticket numbers array
	if (env->ticket_nums)
		free(env->ticket_nums);

	// Destroy other mutexes
	pthread_mutex_destroy(&env->hunger_lock);
	pthread_mutex_destroy(&env->print_mutex);
	pthread_mutex_destroy(&env->meal_mutex);
	pthread_mutex_destroy(&env->start_mutex);
	pthread_mutex_destroy(&env->log_buffer.mutex);

	free(env);
}
