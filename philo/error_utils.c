/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:44:58 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 22:01:12 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file print_error.c
 * @brief Error handling function for philosopher simulation.
 */

#include "philo.h"

/**
 * @brief Prints an error message to standard output.
 *
 * This function writes the given error message to STDOUT_FILENO.
 *
 * @param msg Pointer to the error message string.
 */
void	print_error(char *msg)
{
	size_t	len_msg;

	len_msg = ft_strlen(msg);
	if (write(STDOUT_FILENO, msg, len_msg) == -1)
	{
		print_error("Error: print_error: write failed.\n");
		return ;
	}
}
