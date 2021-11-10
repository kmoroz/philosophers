NAME = philo_bonus
SRC_BONUS = philo.c utils.c \
time.c initialiser.c \
checker.c routine.c \
freeing.c
HEADER_BONUS = philo.h
OBJ_BONUS = $(SRC_BONUS:.c=.o)

FLAGS = -g -pthread -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(HEADER_BONUS) $(OBJ_BONUS)
	@gcc $(FLAGS) $(OBJ_BONUS) -o $(NAME)
	@echo "\n\033[92m $@ built 🍝\033[0m\n"

%.o : %.c $(HEADER_BONUS)
	@gcc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ_BONUS)
	@echo "\n\033[36m object files removed 👋\033[0m\n"

fclean: clean
	@rm -f $(NAME)
	@echo "\n\033[36m executables removed 👋\033[0m\n"

re: fclean all bonus
