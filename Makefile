# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkalia <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/03/23 14:12:11 by jkalia            #+#    #+#              #
#*   Updated: 2017/04/28 23:55:53 by jkalia           ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

NAME		:= ftdb
CC		:= gcc
CFLAGS		+= -Wall -Wextra -Werror
CFLAGS		+= -I includes/ -I libft/includes/
LDFLAGS		:= -L libft/ -lft

LIBFT		:= libft/libft.a

FILES		:= db_dispatch db_delete db_reply db_init db_set db_load db_murmurhash db_get db_server
SRC		:= $(addprefix src/, $(addsuffix .c, $(FILES)))
OBJ		:= $(SRC:.c=.o)

.PHONY = all clean fclean clean re

all: $(NAME) socket

$(LIBFT):
	@make -C libft	

$(OBJ): %.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "\033[32mCompiled Project Object Files\033[0m"

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "\033[32mCreated Executable\033[0m"

clean:
	@make -C libft clean
	@rm -rf $(OBJ)
	@echo "\033[32mRemoved Object Files\033[0m"

fclean: clean
	@make -C libft fclean
	@rm -rf $(NAME)
	@rm -rf client
	@echo "\033[32mRemoved Executable\033[0m"

re: fclean all

socket:
	@gcc src/db_client.c -o client
