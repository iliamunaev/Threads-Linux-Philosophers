/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:55:25 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:17:33 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_env.c
 * @brief Environment initialization functions for philosopher simulation.
 */
#include "philo.h"

/**
 * @brief Initializes mutexes required for synchronization.
 *
 * @param env Pointer to the environment structure.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	init_mutexes(t_env *env)
{
	int	i;

	if (pthread_mutex_init(&env->print_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&env->meal_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&env->start_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&env->end_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_mutex_init(&env->forks[i], NULL) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Allocates memory for philosophers, forks, and tickets.
 *
 * @param env Pointer to the environment structure.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	alloc_mem(t_env *env)
{
	env->forks = malloc(env->num_philo * sizeof(pthread_mutex_t));
	env->philos = malloc(env->num_philo * sizeof(t_philo));
	env->ticket_nums = malloc(env->num_philo * sizeof(int));
	if (!env->forks || !env->philos || !env->ticket_nums)
	{
		print_error("Error: alloc_mem: mem allocation failed\n");
		free(env->forks);
		free(env->philos);
		free(env->ticket_nums);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the log buffer mutex.
 *
 * @param env Pointer to the environment structure.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	init_log_buffer(t_env	*env)
{
	env->log_buffer.count = 0;
	if (pthread_mutex_init(&env->log_buffer.mutex, NULL) != 0)
	{
		print_error("Error: init_log_buffer: mutex init failed\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the environment for the philosopher simulation.
 *
 * Parses arguments, allocates memory, initializes mutexes, and
 * sets initial values.
 *
 * @param env Pointer to the environment structure.
 * @param ac Argument count.
 * @param av Argument values.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int	init_env(t_env *env, int ac, char **av)
{
	env->num_philo = ft_atoi(av[1]);
	env->die_time = ft_atoi(av[2]);
	env->eat_time = ft_atoi(av[3]);
	env->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		env->meals_limit = ft_atoi(av[5]);
	else
		env->meals_limit = -1;
	if (alloc_mem(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_log_buffer(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_mutexes(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	env->ticket_counter = 0;
	env->ended = 0;
	env->start_time = get_time();
	return (EXIT_SUCCESS);
}
