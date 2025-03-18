#include "philo.h"

int	init_print_mutex(t_env *env)
{

	if (pthread_mutex_init(&env->print_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_meal_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->meal_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	init_start_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->start_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
int	init_end_mutex(t_env *env)
{

	if (pthread_mutex_init(&env->end_mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		(void)pthread_mutex_destroy(&env->start_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	init_log_buffer_mutex(t_env *env)
{
	if (pthread_mutex_init(&env->log_buffer.mutex, NULL) != 0)
	{
		(void)pthread_mutex_destroy(&env->print_mutex);
		(void)pthread_mutex_destroy(&env->meal_mutex);
		(void)pthread_mutex_destroy(&env->start_mutex);
		(void)pthread_mutex_destroy(&env->end_mutex);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
int	init_forks_mutex(t_env *env)
{
	int i;

	i = 0;
	while (i < env->num_philo)
	{
		if (pthread_mutex_init(&env->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				(void)pthread_mutex_destroy(&env->forks[i]);
			}
			(void)pthread_mutex_destroy(&env->print_mutex);
			(void)pthread_mutex_destroy(&env->meal_mutex);
			(void)pthread_mutex_destroy(&env->start_mutex);
			(void)pthread_mutex_destroy(&env->end_mutex);
			(void)pthread_mutex_destroy(&env->log_buffer.mutex);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
