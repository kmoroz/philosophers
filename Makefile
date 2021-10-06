PHILO = philo
SRC = src/philo/philo.c src/philo/initialiser.c \
src/philo/utils.c src/philo/time.c \
src/philo/checker.c src/philo/freeing.c
HEADER = src/philo/philo.h
OBJ = $(SRC:.c=.o)

FLAGS = -g -pthread -Wall -Werror -Wextra

all: $(PHILO)

$(PHILO): $(HEADER) $(OBJ)
	@gcc $(FLAGS) $(OBJ) -o $(PHILO)
	@echo "\n\033[92m $@ built 🍝\033[0m\n"

%.o : %.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "\n\033[36m object files removed 👋\033[0m\n"

fclean: clean
	@rm -f $(PHILO)
	@echo "\n\033[36m executables removed 👋\033[0m\n"

re: fclean all
