/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:57 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 15:36:38 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Implementation of the philosopher dining problem.
 */
#include "philo.h"

/**
 * @brief Main function to run the philosopher dining problem simulation.
 *
 * @param ac Argument count.
 * @param av Argument values.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
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
