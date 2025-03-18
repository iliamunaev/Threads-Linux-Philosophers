/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:44:58 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 17:05:15 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file error_utils.c
 * @brief Error handling utilities for the philosopher simulation.
 *
 * This file contains functions for handling and reporting errors
 * during the execution of the philosopher simulation.
 */

#include "philo.h"

/**
* @brief Prints an error message to standard output.
*
* This function writes the provided error message to STDOUT.
* If the write operation fails, it recursively attempts to report
* the failure by calling itself, which may lead to infinite recursion
* in extreme cases.
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
