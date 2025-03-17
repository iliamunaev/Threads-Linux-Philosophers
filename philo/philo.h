/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:13:14 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 19:01:00 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>

# define LOG_BUFFER_SIZE 1024

typedef struct s_env	t_env;

/**
 * @struct s_log_entry
 * @brief Represents an individual log entry.
 */
typedef struct s_log_entry
{
	long	timestamp;
	int		id;
	char	status[32];
}	t_log_entry;

/**
 * @struct s_log_buffer
 * @brief Thread-safe circular buffer to batch log entries.
 */
typedef struct s_log_buffer
{
	t_log_entry		entries[LOG_BUFFER_SIZE];
	int				count;
	pthread_mutex_t	mutex;
}	t_log_buffer;

/**
 * @struct s_philo
 * @brief Represents a single philosopher in the simulation.
 */
typedef struct s_philo
{
	int			id;
	int			meals;
	long		last_meal;
	pthread_t	thread;
	t_env		*env;
}	t_philo;

/**
 * @struct s_env
 * @brief Shared simulation environment and configuration.
 */
typedef struct s_env
{
	int				num_philo;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	int				meals_limit;
	int				ended;
	long			start_time;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				*ticket_nums;
	int				ticket_counter;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	start_mutex;
	t_log_buffer	log_buffer;
}	t_env;

// init
int		init_env(t_env *env, int ac, char **av);
void	setup_philos(t_env *env);
int		start_threads(t_env *env, pthread_t *mon, pthread_t *logger_thread);
void	join_threads(t_env *env, pthread_t mon, pthread_t logger_thread);

long	get_time(void);
void	precise_sleep(long ms);
void	print_status(t_philo *p, const char *status);
void	take_forks(t_philo *p);
void	put_forks(t_philo *p);
void	*log_flusher(void *arg);

void	done_eating(t_philo *p);

void	*routine(void *arg);
void	*monitor(void *arg);

// utils
int		ft_atoi(const char *str);
void	print_error(char *msg);
void	clean_up(t_env *env);
void	clean_env(t_env *env);
void	ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *s);
#endif
