NAME = philo
SRC = philo.c initialiser.c \
utils.c time.c \
checker.c freeing.c \
routine.c
HEADER = philo.h
OBJ = $(SRC:.c=.o)

FLAGS = -g -pthread -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(HEADER) $(OBJ)
	@gcc $(FLAGS) $(OBJ) -o $(NAME)
	@echo "\n\033[92m $@ built 🍝\033[0m\n"

%.o : %.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "\n\033[36m object files removed 👋\033[0m\n"

fclean: clean
	@rm -f $(NAME)
	@echo "\n\033[36m executables removed 👋\033[0m\n"

re: fclean all

.PHONY: all clean fclean re