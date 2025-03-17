/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_routin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:44:14 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 14:03:09 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_routin(t_env *env, pthread_t *mon)
{
	int			i;
	
	pthread_mutex_lock(&env->start_mutex);
	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: start_routin: failed to create philosopher thread\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);	
	return (EXIT_SUCCESS);
}