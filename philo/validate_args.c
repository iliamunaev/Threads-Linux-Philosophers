/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:14:05 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:26:26 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file validate_args.c
 * @brief Command-line argument validation for the philosopher simulation.
 *
 * This file contains functions to verify that user-provided arguments
 * are correctly formatted and within valid constraints.
 */

#include "philo.h"

/**
 * @brief Determines whether a character is a numeric digit.
 *
 * This function checks if the character `c` is a digit ('0' through '9').
 *
 * @param c The character to check.
 * @return int Returns nonzero if `c` is a numeric digit, otherwise 0.
 */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief Validates the command-line arguments for the philosopher simulation.
 *
 * This function ensures:
 * - The correct number of arguments is provided (`./philo num die eat sleep [meals]`).
 * - Each argument consists only of numeric digits.
 * - The number of philosophers is at least 1.
 *
 * Thread safety:
 * - This function operates on read-only input (`av[]`) and does not modify shared data.
 *
 * @param ac The number of command-line arguments.
 * @param av The array of argument strings.
 * @return bool Returns `true` if all arguments are valid, otherwise `false`.
 */
bool	validate_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6 || (ft_atoi(av[1]) < 1) || !av[1][0])
		return (false);
	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			if (!ft_isdigit(av[i][j]))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}
