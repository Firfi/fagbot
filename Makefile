CC = gcc

all:
	$(CC) -Wall -Werror -Wextra -o bot main.c sock.c
