/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_flusher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:14 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 12:06:59 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	buffered_print(t_log_buffer *buffer, long timestamp, int id, const char *status)
{
	pthread_mutex_lock(&buffer->mutex);
	if (buffer->count < LOG_BUFFER_SIZE)
	{
		buffer->entries[buffer->count].timestamp = timestamp;
		buffer->entries[buffer->count].id = id;
		ft_strncpy(buffer->entries[buffer->count].status, status, sizeof(buffer->entries[buffer->count].status) - 1);
		buffer->entries[buffer->count].status[sizeof(buffer->entries[buffer->count].status) - 1] = '\0';
		buffer->count++;
	}
	pthread_mutex_unlock(&buffer->mutex);
}


void	print_status(t_philo *p, const char *status)
{
	long	timestamp;
	
	timestamp = get_time() - p->env->start_time;
	buffered_print(&p->env->log_buffer, timestamp, p->id + 1, status);
}


void	*log_flusher(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	while (!env->ended || env->log_buffer.count > 0)
	{
		pthread_mutex_lock(&env->log_buffer.mutex);
		i = 0;
		while (i < env->log_buffer.count)
		{
			printf("%ld %d %s\n",
					env->log_buffer.entries[i].timestamp,
					env->log_buffer.entries[i].id,
					env->log_buffer.entries[i].status);
					i++;			
		}
		env->log_buffer.count = 0;
		pthread_mutex_unlock(&env->log_buffer.mutex);
		usleep(1000); // flush every 1ms
	}
	return (NULL);
}
