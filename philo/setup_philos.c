/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:23:40 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:12:18 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file setup_philos.c
 * @brief Initializes philosopher structures in the simulation.
 */
#include "philo.h"

/**
 * @brief Initializes philosopher data structures.
 *
 * Sets up each philosopher with an ID, initializes their meal count,
 * sets their last meal time, and links them to the environment.
 *
 * @param env Pointer to the environment structure.
 */

void	setup_philos(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		env->philos[i].id = i;
		env->philos[i].meals = 0;
		env->philos[i].last_meal = env->start_time;
		env->philos[i].env = env;
		i++;
	}
}
