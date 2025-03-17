# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 09:44:25 by imunaev-          #+#    #+#              #
#    Updated: 2025/03/17 17:55:40 by imunaev-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS =	main.c \
		clean_up.c \
		error_utils.c \
		init_env.c \
		join_threads.c \
		log_flusher.c \
		monitor.c \
		philo_routin.c \
		philo.c \
		setup_philos.c \
		start_routin.c \
		start_threads.c \
		utils.c

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
