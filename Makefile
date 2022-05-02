SRCS		=	philo.c		\
				#utils.c

OBJS		=	$(SRCS:.c=.o)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror -pthread -fsanitize=thread

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
