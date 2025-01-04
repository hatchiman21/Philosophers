SRC_DIR = src_bonus
OBJS_DIR = obj_bonus

SRCS = main routine philo_utils philo_errors philo_starvation_checks setup_forks intiate_philo
SRCS:= $(addprefix $(SRC_DIR)/, $(SRCS))
SRCS:= $(addsuffix _bonus.c, $(SRCS))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

NAME = philo_bonus

all: $(NAME)

$(NAME): $(OBJS) philo_bonus.h
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
