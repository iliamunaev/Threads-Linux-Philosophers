/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:57 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:56:53 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Implementation of the philosopher dining problem.
 */
#include "philo.h"

/**
 * @brief Validates the command-line arguments.
 *
 * @param ac Argument count.
 * @return EXIT_SUCCESS if valid, otherwise EXIT_FAILURE.
 */
static int	validate_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		print_error("Usage: ./philo num die eat sleep [meals]\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes and sets up the environment.
 *
 * @param env Pointer to the environment structure.
 * @param ac Argument count.
 * @param av Argument values.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
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
