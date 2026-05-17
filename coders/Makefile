NAME = codexion

SRC = main.c \
	  error.c \
	  parser.c \
	  coder.c \
	  dongle.c \
	  time.c \
	  simulation.c \
	  request.c \
	  release.c \
	  log.c \
	  coder_routine.c \
	  monitor.c \
	  heap.c

INCLUDE = include
HEADER = $(INCLUDE)/codexion.h

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

lint:
	norminette $(SRC)

re: fclean all

.PHONY: all clean fclean re
