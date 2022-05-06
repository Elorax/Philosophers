#SRCS		=	philo.c					\
				arg_handlers.c			\
				ft_atoi.c				\
				routine.c				\
				threads_handlers.c		\
				timers.c				\

SRCS		=	philo.c					\
				arg_handlers.c			\
				ft_atoi.c				\
				routine.c				\
				threads_handlers.c		\
				timers.c				\
				true_steps1.c			\
				true_steps2.c			\
				true_death_checker.c	

SRCS_DEBUG	=	philo.c					\
				arg_handlers.c			\
				ft_atoi.c				\
				routine.c				\
				threads_handlers.c		\
				timers.c				\
				debug_steps1.c			\
				debug_steps2.c			\
				debug_death_checker.c	
				


OBJS		=	$(SRCS:.c=.o)
OBJS_DEBUG	=	$(SRCS_DEBUG:.c=.o)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror -g -pthread #-fsanitize=thread

NAME		=	philo
NAME_DEBUG	=	philo_debug

%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@


$(NAME):		$(OBJS)
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(NAME_DEBUG):	$(OBJS_DEBUG)
					$(CC) $(CFLAGS) $(OBJS_DEBUG) -o $(NAME_DEBUG)
				

all:			$(NAME)

debug:			$(NAME_DEBUG)

clean:
				rm -rf $(OBJS) $(OBJS_DEBUG)

fclean:			clean
				rm -rf $(NAME) $(NAME_DEBUG)

re:				fclean all

.PHONE:			all clean fclean re debug
