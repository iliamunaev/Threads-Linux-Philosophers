#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(200);
}

void	print_status(t_philo *p, char *status)
{
	pthread_mutex_lock(&p->env->print_mutex);
	if (!p->env->ended)
		printf("%ld %d %s\n", get_time() - p->env->start_time, p->id + 1, status);
	pthread_mutex_unlock(&p->env->print_mutex);
}

void	take_forks(t_philo *p)
{
	int left;
	int right;

	left = p->id;
	right = (p->id + 1) % p->env->num_philo;
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->env->forks[left]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->env->forks[right]);
		print_status(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->env->forks[right]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->env->forks[left]);
		print_status(p, "has taken a fork");
	}
}

void	put_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->env->forks[p->id]);
	pthread_mutex_unlock(&p->env->forks[(p->id + 1) % p->env->num_philo]);
}

/*
 *   1. Assign philosopher p->id a 'ticket' (env->ticket_counter++).
 *   2. While ticket is WAY bigger than neighbors’ tickets
 *      (meaning I'd be starving them),
 *      wait on a condition variable.
 */
void	neighbors_starvation_wait(t_philo *p)
{
	t_env	*env;
	int		left;
	int		right;

	env = p->env;
	left = (p->id + env->num_philo - 1) % env->num_philo;
	right = (p->id + 1) % env->num_philo;
	pthread_mutex_lock(&env->hunger_lock);
	env->ticket_nums[p->id] = env->ticket_counter;
	env->ticket_counter++;
	while (env->ticket_nums[p->id] - env->ticket_nums[left]  > THRESH * env->num_philo
		|| env->ticket_nums[p->id] - env->ticket_nums[right] > THRESH * env->num_philo)
	{
		pthread_cond_wait(&env->cond_vars[p->id], &env->hunger_lock);
	}
	pthread_mutex_unlock(&env->hunger_lock);
}

/*
 *   1. After putting forks down, set my ticket to HIGH_SENTINEL,
 *      meaning I'm no longer hungry.
 *   2. Signal left & right neighbors in case they were waiting.
 */
void	done_eating(t_philo *p)
{
	t_env	*env;
	int		left;
	int		right;

	env = p->env;
	left = (p->id + env->num_philo - 1) % env->num_philo;
	right = (p->id + 1) % env->num_philo;
	pthread_mutex_lock(&env->hunger_lock);
	env->ticket_nums[p->id] = HIGH_SENTINEL;
	pthread_cond_signal(&env->cond_vars[left]);
	pthread_cond_signal(&env->cond_vars[right]);
	pthread_mutex_unlock(&env->hunger_lock);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	pthread_mutex_lock(&p->env->start_mutex);
	pthread_mutex_unlock(&p->env->start_mutex);

	// Optional "stagger" if #philo is odd and I'm philosopher 0.
	   if (p->env->num_philo % 2 && p->id == 0)
		   precise_sleep(2 * p->env->eat_time);
		if (((p->env->num_philo % 2) == 0) && p->id % 2)
		   precise_sleep(p->env->eat_time);


	while (!p->env->ended
		&& (p->env->meals_limit == -1 || p->meals < p->env->meals_limit))
	{
		neighbors_starvation_wait(p);
		take_forks(p);

		pthread_mutex_lock(&p->env->meal_mutex);
		p->last_meal = get_time();
		pthread_mutex_unlock(&p->env->meal_mutex);

		print_status(p, "is eating");
		precise_sleep(p->env->eat_time);
		p->meals++;

		put_forks(p);
		done_eating(p);

		print_status(p, "is sleeping");
		precise_sleep(p->env->sleep_time);

		print_status(p, "is thinking");
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_env	*env;
	int		full;
	int		i;

	env = (t_env *)arg;
	while (!env->ended)
	{
		full = 0;
		i = 0;
		while (i < env->num_philo)
		{
			pthread_mutex_lock(&env->meal_mutex);
			long time_since_meal = get_time() - env->philos[i].last_meal;
			int  mcount = env->philos[i].meals;
			pthread_mutex_unlock(&env->meal_mutex);

			/* Check death */
			if (time_since_meal > env->die_time)
			{
				pthread_mutex_lock(&env->print_mutex);
				if (!env->ended)
				{
					env->ended = 1;
					printf("%ld %d died\n", get_time() - env->start_time, i + 1);
				}
				pthread_mutex_unlock(&env->print_mutex);
				return (NULL);
			}
			if (env->meals_limit != -1 && mcount >= env->meals_limit)
				full++;
			i++;
		}
		if (env->meals_limit != -1 && full == env->num_philo)
			env->ended = 1;
		usleep(500);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_env	env;
	int		i;
	pthread_t mon;

	if (ac < 5 || ac > 6)
		return (printf("Usage: ./philo num die eat sleep [meals]\n"), 1);

	memset(&env, 0, sizeof(env));
	env.num_philo = atoi(av[1]);
	env.die_time = atoi(av[2]);
	env.eat_time = atoi(av[3]);
	env.sleep_time = atoi(av[4]);
	env.meals_limit = (ac == 6) ? atoi(av[5]) : -1;

	env.forks = malloc(sizeof(pthread_mutex_t) * env.num_philo);
	env.philos = malloc(sizeof(t_philo) * env.num_philo);
	env.cond_vars = malloc(sizeof(pthread_cond_t) * env.num_philo);
	env.ticket_nums = malloc(sizeof(int) * env.num_philo);

	pthread_mutex_init(&env.print_mutex, NULL);
	pthread_mutex_init(&env.meal_mutex, NULL);
	pthread_mutex_init(&env.start_mutex, NULL);
	pthread_mutex_init(&env.hunger_lock, NULL);

	i = 0;
	while (i < env.num_philo)
	{
		pthread_mutex_init(&env.forks[i], NULL);
		pthread_cond_init(&env.cond_vars[i], NULL);
		env.ticket_nums[i] = HIGH_SENTINEL;
		i++;
	}
	env.ticket_counter = 0;

	env.start_time = get_time();

	/* Initialize philosopher structs */
	i = 0;
	while (i < env.num_philo)
	{
		env.philos[i].id = i;
		env.philos[i].meals = 0;
		env.philos[i].last_meal = env.start_time;
		env.philos[i].env = &env;
		i++;
	}

	/* Lock start_mutex so all threads block until we unlock. */
	pthread_mutex_lock(&env.start_mutex);

	/* Create philosopher threads */
	i = 0;
	while (i < env.num_philo)
	{
		pthread_create(&env.philos[i].thread, NULL, routine, &env.philos[i]);
		i++;
	}

	/* Create monitor thread */
	pthread_create(&mon, NULL, monitor, &env);

	/* Release the philosophers */
	pthread_mutex_unlock(&env.start_mutex);

	/* Wait for philosopher threads */
	i = 0;
	while (i < env.num_philo)
	{
		pthread_join(env.philos[i].thread, NULL);
		i++;
	}

	/* Wait for monitor thread to exit */
	pthread_join(mon, NULL);

	/* Cleanup */
	i = 0;
	while (i < env.num_philo)
	{
		pthread_mutex_destroy(&env.forks[i]);
		pthread_cond_destroy(&env.cond_vars[i]);
		i++;
	}
	free(env.forks);
	free(env.philos);
	free(env.cond_vars);
	free(env.ticket_nums);

	pthread_mutex_destroy(&env.hunger_lock);
	pthread_mutex_destroy(&env.print_mutex);
	pthread_mutex_destroy(&env.meal_mutex);
	pthread_mutex_destroy(&env.start_mutex);

	return (EXIT_SUCCESS);
}
