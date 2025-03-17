/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:47:57 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 17:11:20 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file philo.c
 * @brief Implementation of the philosopher dining problem.
 */
#include "philo.h"

/**
 * @brief Validates the command-line arguments.
 *
 * @param ac Argument count.
 * @return EXIT_SUCCESS if valid, otherwise EXIT_FAILURE.
 */
static int	validate_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		print_error("Usage: ./philo num die eat sleep [meals]\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
 * @brief Creates a logger thread.
 *
 * @param env Pointer to the environment structure.
 * @param logger_thread Pointer to the logger thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_logger_thread(t_env *env, pthread_t *logger_thread)
{
	if (pthread_create(logger_thread, NULL, &log_flusher, env) != 0)
	{
		print_error("Error: Failed to create logger thread\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates philosopher threads.
 *
 * @param env Pointer to the environment structure.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_philosopher_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_create(&env->philos[i].thread, NULL, routine, &env->philos[i]) != 0)
		{
			print_error("Error: Failed to create philosopher thread\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates the monitor thread.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the monitor thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	create_monitor_thread(t_env *env, pthread_t *mon)
{
	if (pthread_create(mon, NULL, monitor, env) != 0)
	{
		print_error("Error: Failed to create monitor thread\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Starts all required threads.
 *
 * @param env Pointer to the environment structure.
 * @param mon Pointer to the monitor thread.
 * @param logger_thread Pointer to the logger thread.
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int	start_threads(t_env *env, pthread_t *mon, pthread_t *logger_thread)
{
	pthread_mutex_lock(&env->start_mutex);
	if (create_logger_thread(env, logger_thread) == EXIT_FAILURE)
	{
		clean_up(env);
		return (EXIT_FAILURE);
	}
	if (create_philosopher_threads(env) == EXIT_FAILURE)
	{
		pthread_mutex_unlock(&env->start_mutex);
		clean_up(env);
		return (EXIT_FAILURE);
	}
	if (create_monitor_thread(env, mon) == EXIT_FAILURE)
	{
		clean_up(env);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&env->start_mutex);
	return (EXIT_SUCCESS);
}

/**
 * @brief Joins all created threads.
 *
 * @param env Pointer to the environment structure.
 * @param mon Monitor thread.
 * @param logger_thread Logger thread.
 */
static void	join_threads(t_env *env, pthread_t mon, pthread_t logger_thread)
{
	int	i;

	i = 0;
	while (i < env->num_philo)
	{
		pthread_join(env->philos[i].thread, NULL);
		i++;
	}
	pthread_join(mon, NULL);
	pthread_join(logger_thread, NULL);
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

	if (validate_args(ac) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_and_setup(&env, ac, av) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_threads(env, &mon, &logger_thread) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	join_threads(env, mon, logger_thread);
	clean_up(env);
	return (EXIT_SUCCESS);
}
