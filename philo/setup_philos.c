/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:23:40 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 13:20:41 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
