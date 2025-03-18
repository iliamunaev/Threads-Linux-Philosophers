/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:57 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:15:17 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Entry point for the philosopher dining simulation.
 *
 * This file contains the `main` function, which initializes the environment,
 * validates arguments, starts necessary threads, and manages cleanup.
 */

#include "philo.h"

/**
 * @brief Main function to initialize and start the philosopher simulation.
 *
 * - Validates command-line arguments.
 * - Initializes the simulation environment.
 * - Starts philosopher, monitor, and logger threads.
 * - Waits for threads to finish and cleans up resources.
 *
 * @param ac Argument count.
 * @param av Argument vector.
 * @return int Returns EXIT_SUCCESS on successful execution, or EXIT_FAILURE
 * if initialization or thread creation fails.
 */
int	main(int ac, char **av)
{
	t_env		*env;
	pthread_t	mon;
	pthread_t	logger_thread;

	if (!validate_args(ac, av))
	{
		print_error("Usage (only digits): ./philo num die eat sleep [meals]\n");
		return (EXIT_FAILURE);
	}
	init_program(&env, ac, av);
	mon = 0;
	logger_thread = 0;
	if (start_threads(env, &mon, &logger_thread) == EXIT_FAILURE)
	{
		print_error("Error: main: start_threads failed\n");
		join_threads(env, mon, logger_thread);
		free_all(env);
		return (EXIT_FAILURE);
	}
	join_threads(env, mon, logger_thread);
	free_all(env);
	return (EXIT_SUCCESS);
}
