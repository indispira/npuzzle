NAME =		npuzzle.so
CC =			cc
CFLAGS = 	-Wall -Werror -Wextra -g -std=c99 -O3 -D_GNU_SOURCE -fsanitize=address
# LIBS = 		-lm
SRCS =		npuzzle.c
OBJS =		npuzzle.o

all :		$(NAME)

$(NAME) :
			$(CC) -fPIC -c $(CFLAGS) $(SRCS) #$(LIBS) $(SRCS)
			$(CC) -shared -o npuzzle.so $(OBJS) -lc
			# ar rc $(NAME) $(OBJS)
			# ranlib $(NAME)

clean :
			rm -f $(OBJS)

fclean :	clean
			rm -f $(NAME)

re :		fclean all

.PHONY : all clean fclean re
