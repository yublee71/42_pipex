# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yublee <yublee@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/19 14:55:22 by yublee            #+#    #+#              #
#    Updated: 2024/02/22 11:30:57 by yublee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#needs editing later
DIRS := src/action

CC = cc

CFLAGS = -Wall -Wextra -Werror

RM = rm

RMFLAGS = -fv

AR = ar

ARFLAGS = rcs

INCLUDE = -I include/

NAME = pipex

#needs editing later
SRC = src/main.c \

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

BUILD_DIR = build

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(BUILD_DIR) $(OBJ)
	$(CC) -L$(LIBFT_DIR) $(OBJ) -g -o $@ -lft

$(LIBFT_LIB):
	make -C $(LIBFT_DIR)

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%,$(DIRS))

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	$(RM) -rf $(BUILD_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)
	$(RM) $(RMFLAGS) $(LIBFT_LIB)

re: fclean all

.PHONY: all clean fclean re
