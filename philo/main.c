/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:57 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 18:24:06 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Implementation of the philosopher dining problem.
 */
#include "philo.h"

/**
 * @brief Checks if a character is a numeric digit.
 *
 * This function tests whether the given character `c` is a numeric
 * digit (characters '0' through '9').
 *
 * @param c The character to check.
 * @return int Nonzero if the character is a numeric digit, 0 otherwise.
 */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief Validates the command-line arguments.
 *
 * Ensures the correct number of arguments is provided and that each argument
 * consists only of numeric digits.
 *
 * @param ac Argument count.
 * @param av Argument vector.
 * @return true if valid, false otherwise.
 */
static bool	validate_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6 || (ft_atoi(av[1]) < 1) || !av[1][0])
		return (false);
	i = 1;
	while(i < ac)
	{
		j = 0;
		while(av[i][j] != '\0')
		{
			if (!ft_isdigit(av[i][j]))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
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

	if (!validate_args(ac, av))
	{
		print_error("Usage (only digits): ./philo num die eat sleep [meals]\n");
		return (EXIT_FAILURE);
	}
	if (init_and_setup(&env, ac, av) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_threads(env, &mon, &logger_thread) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	join_threads(env, mon, logger_thread);
	clean_up(env);
	return (EXIT_SUCCESS);
}
