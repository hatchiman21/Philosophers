SRC_DIR = src
OBJS_DIR = obj

SRCS = main.c philo_life.c philo_utils.c philo_errors.c philo_starvation_checks.c
SRCS:= $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

NAME = philo

all: $(NAME)

$(NAME): $(OBJS) philo.h
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
			$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
			mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
