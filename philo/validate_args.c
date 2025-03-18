/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:14:05 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:14:06 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
