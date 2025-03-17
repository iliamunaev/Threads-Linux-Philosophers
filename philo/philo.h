#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
#include <string.h>

# define HIGH_SENTINEL 2147483647
# define THRESH        5

/*
** Forward declare t_env so t_philo can reference it.
*/
typedef struct s_env t_env;

/*
** Philosopher structure
*/
typedef struct s_philo {
    int         id;
    int         meals;
    long        last_meal;
    pthread_t   thread;
    t_env       *env;
}   t_philo;

/*
** Environment structure
*/
typedef struct s_env {
    int             num_philo;
    long            die_time;
    long            eat_time;
    long            sleep_time;
    int             meals_limit;
    int             ended;
    long            start_time;

    /* Forks array */
    pthread_mutex_t *forks;

    /* All philosophers */
    t_philo         *philos;

    /* Hybrid approach fields */
    pthread_cond_t  *cond_vars;
    int             *ticket_nums;
    int             ticket_counter;
    pthread_mutex_t hunger_lock;

    /* Misc. locks */
    pthread_mutex_t print_mutex;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t start_mutex;
}   t_env;

/* Function prototypes from philo.c */
long    get_time(void);
void    precise_sleep(long ms);
void    print_status(t_philo *p, char *status);
void    take_forks(t_philo *p);
void    put_forks(t_philo *p);

void    neighbors_starvation_wait(t_philo *p);
void    done_eating(t_philo *p);

void    *routine(void *arg);
void    *monitor(void *arg);

#endif
