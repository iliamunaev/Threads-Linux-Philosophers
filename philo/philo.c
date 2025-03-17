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
		usleep(500);
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

		if (p->env->num_philo % 2 && p->id == 0)
		{
			print_status(p, "is thinking");
		   precise_sleep(2 * p->env->eat_time);		
		}
		if (p->env->num_philo % 2 && p->id % 2)
		{
			print_status(p, "is thinking");
		   precise_sleep(p->env->eat_time);
		}
		else if (((p->env->num_philo % 2) == 0) && p->id % 2)
		{
			print_status(p, "is thinking");
		   precise_sleep(p->env->eat_time);
		}

	while (!p->env->ended
		&& (p->env->meals_limit == -1 || p->meals < p->env->meals_limit))
	{
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
		usleep(500);
	}
	return (NULL);
}
