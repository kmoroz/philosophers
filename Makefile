PHILO = philo
PHILO_BONUS = philo_bonus
SRC = src/philo/philo.c src/philo/initialiser.c \
src/philo/utils.c src/philo/time.c \
src/philo/checker.c src/philo/freeing.c
SRC_BONUS = src/philo_bonus/philo.c
HEADER = src/philo/philo.h
HEADER_BONUS = src/philo_bonus/philo.h
OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

FLAGS = -g -pthread -Wall -Werror -Wextra

all: $(PHILO)

$(PHILO): $(HEADER) $(OBJ)
	@gcc $(FLAGS) $(OBJ) -o $(PHILO)
	@echo "\n\033[92m $@ built 🍝\033[0m\n"

bonus: $(HEADER_BONUS) $(OBJ_BONUS)
	@gcc $(FLAGS) $(OBJ_BONUS) -o $(PHILO_BONUS)
	@echo "\n\033[92m $@ built 🍝\033[0m\n"

%.o : %.c $(HEADER) $(HEADER_BONUS)
	@gcc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "\n\033[36m object files removed 👋\033[0m\n"

fclean: clean
	@rm -f $(PHILO) $(PHILO_BONUS)
	@echo "\n\033[36m executables removed 👋\033[0m\n"

re: fclean all bonus
