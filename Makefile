# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gribeiro <gribeiro@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 19:38:58 by gribeiro          #+#    #+#              #
#    Updated: 2025/03/15 23:16:59 by gribeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	@if [ -f infile ]; then \
		rm -f infile; \
	fi
	@if [ -f outfile ]; then \
		rm -f outfile; \
	fi
	@if [ -d 42_pipex_tester ]; then \
		rm -rf 42_pipex_tester; \
	fi
	@if [ -d pipex-tester ]; then \
		rm -rf pipex-tester; \
	fi

re: fclean all

test: all
	@if [ ! -d pipex-tester ]; then \
		git clone https://github.com/vfurmane/pipex-tester; \
	fi	
	@cd pipex-tester && ./run.sh
	@if [ ! -d 42_pipex_tester ]; then \
		git clone https://github.com/michmos/42_pipex_tester.git; \
	fi
	@cd 42_pipex_tester && bash run.sh --hide-err-log

valgrind: all
	@touch infile
	@valgrind --leak-check=full --track-origins=yes ./pipex infile "ls -l" "wc -l" outfile