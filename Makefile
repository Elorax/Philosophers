SRCS		=	philo.c				\
				arg_handlers.c		\
				ft_atoi.c			\
				death_checker.c		\
				routine.c			\
				steps1.c			\
				steps2.c			\
				threads_handlers.c	\
				timers.c

OBJS		=	$(SRCS:.c=.o)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror -g -pthread #-fsanitize=thread

NAME		=	Philosophers

%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@


$(NAME):	$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONE:		all clean fclean re
