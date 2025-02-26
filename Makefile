# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gribeiro <gribeiro@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 19:38:58 by gribeiro          #+#    #+#              #
#    Updated: 2025/02/26 02:16:28 by gribeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	@touch outfile
	./pipex infile "ls -l" "wc -l" outfile

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./pipex infile "ls -l" "wc -l" outfile