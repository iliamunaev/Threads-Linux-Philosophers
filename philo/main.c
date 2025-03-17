/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:44:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 11:57:57 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_env	*env;
	int		i;
	pthread_t mon;


	if (ac < 5 || ac > 6)
		return (printf("Usage: ./philo num die eat sleep [meals]\n"), 1);
	env = malloc(sizeof(t_env));
	if (!env)
	{
		print_error("Error: main: env mem alloc failed\n");
		return (EXIT_FAILURE);
	}
	if (init_env(env, ac, av) == 1)
	{
		clean_up(env);
		return (EXIT_FAILURE);
	}
	init_philos(env);

	/* Lock start_mutex so all threads block until we unlock. */
	pthread_mutex_lock(&env->start_mutex);

	/* Create philosopher threads */
	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: Failed to create philosopher thread\n");
			clean_up(env);
			return (EXIT_FAILURE);
		}
		i++;
	}
	
	// Create monitor thread
	if (pthread_create(&mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		clean_up(env);
		return (EXIT_FAILURE);
	}

	// Release the philosophers
	pthread_mutex_unlock(&env->start_mutex);

	// Wait for philosopher threads
	i = 0;
	while (i < env->num_philo)
	{
		pthread_join(env->philos[i].thread, NULL);
		i++;
	}

	// Wait for monitor thread to exit
	pthread_join(mon, NULL);

	clean_up(env);

	return (EXIT_SUCCESS);
}
