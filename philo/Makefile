# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 09:44:25 by imunaev-          #+#    #+#              #
#    Updated: 2025/03/18 17:31:20 by imunaev-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS =	main.c \
		error_utils.c \
		init_env.c \
		init_mutexes_1.c \
		init_mutexes_2.c \
		init_program.c \
		join_threads.c \
		log_flusher.c \
		memory_managment.c \
		monitor.c \
		philo_routin.c \
		philo.c \
		start_threads.c \
		utils.c
		validate_args.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g #-fsanitize=thread #-O3
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
