/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:42:18 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 14:49:58 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		print_error("Usage: ./philo num die eat sleep [meals]\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	init_and_setup(t_env **env, int ac, char **av)
{
	*env = malloc(sizeof(t_env));
	if (!*env)
	{
		print_error("Error: main: env mem alloc failed\n");
		return (EXIT_FAILURE);
	}
	if (init_env(*env, ac, av) == EXIT_FAILURE)
	{
		clean_up(*env);
		return (EXIT_FAILURE);
	}
	setup_philos(*env);
	return (EXIT_SUCCESS);
}

static int	start_threads(t_env *env, pthread_t *mon, pthread_t *logger_thread)
{
	int	i;

	pthread_mutex_lock(&env->start_mutex);

	if (pthread_create(logger_thread, NULL, &log_flusher, env) != 0)
	{
		print_error("Error: Failed to create logger thread\n");
		clean_up(env);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: Failed to create philosopher thread\n");
			pthread_mutex_unlock(&env->start_mutex);
			clean_up(env);
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		clean_up(env);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}

static void	join_threads(t_env *env, pthread_t mon, pthread_t logger_thread)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		pthread_join(env->philos[i].thread, NULL);
		i++;
	}
	pthread_join(mon, NULL);
	pthread_join(logger_thread, NULL);
}

int	main(int ac, char **av)
{
	t_env		*env;
	pthread_t	mon;
	pthread_t logger_thread;

	if (validate_args(ac) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_and_setup(&env, ac, av) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_threads(env, &mon, &logger_thread) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	join_threads(env, mon, logger_thread);
	clean_up(env);
	return (EXIT_SUCCESS);
}
