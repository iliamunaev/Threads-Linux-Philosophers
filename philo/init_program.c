/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:11:18 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:12:30 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_program.c
 * @brief Program initialization for the philosopher simulation.
 *
 * This file contains the function responsible for allocating and initializing
 * the environment (`t_env`), setting up all required resources for
 * the simulation.
 */

#include "philo.h"

/**
 * @brief Allocates and initializes the program environment.
 *
 * This function dynamically allocates memory for the environment (`t_env`)
 * and initializes all required parameters, including mutexes, philosopher
 * structures, and simulation settings.
 *
 * If memory allocation fails, the program prints an error message and
 * immediately exits with EXIT_FAILURE. If environment initialization fails,
 * it frees allocated resources before exiting.
 *
 * @param env Pointer to the environment structure pointer (`t_env **`).
 * @param ac Argument count from the command-line input.
 * @param av Argument vector containing simulation parameters.
 */
void	init_program(t_env **env, int ac, char **av)
{
	*env = malloc(sizeof(t_env));
	if (!*env)
	{
		print_error("Error: init_and_setup: env mem alloc failed\n");
		exit (EXIT_FAILURE);
	}
	if (init_env(*env, ac, av) == EXIT_FAILURE)
	{
		free_env(*env);
		exit (EXIT_FAILURE);
	}
}
