# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 09:44:25 by imunaev-          #+#    #+#              #
#    Updated: 2025/03/17 13:36:20 by imunaev-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS =	main.c \
		philo.c \
		init_env.c \
		setup_philos.c \
		monitor.c \
		log_flusher.c \
		utils.c \
		clean_up.c
		
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
